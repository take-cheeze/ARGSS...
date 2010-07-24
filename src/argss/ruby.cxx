//////////////////////////////////////////////////////////////////////////////////
/// ARGSS - Copyright (c) 2009 - 2010, Alejandro Marzini (vgvgf)
///         All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///     * Redistributions of source code must retain the above copyright
///       notice, this list of conditions and the following disclaimer.
///     * Redistributions in binary form must reproduce the above copyright
///       notice, this list of conditions and the following disclaimer in the
///       documentation and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY
/// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
/// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
/// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <boost/format.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <cassert>
#include <cstdio>

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "ruby.hxx"

#include "../filefinder.hxx"
#include "../fileio.hxx"
#include "../options.hxx"
#include "../system.hxx"
#include "../player.hxx"
#include "../output.hxx"

#ifndef RUBY_VERSION_1_8
	#define ruby_errinfo rb_errinfo()
#endif

static int const BACK_TRACE_LINE_NUM = 10;


namespace ARGSS
{
	void defineMethodsImplement(VALUE klassID, FuncTableElement const* table, unsigned int elmNum)
	{
		assert( elmNum );
		for(unsigned int i = 0; i < elmNum; i++) {
			FuncTableElement const& elm = table[i];

			rb_define_method(klassID, elm.name, elm.func, elm.argc);
			// std::cout << elm.name << std::endl;
		}
	}
	void defineAttributesImplement(VALUE klassID, AttrTableElement const* table, unsigned int elmNum)
	{
		assert( elmNum );
		for(unsigned int i = 0; i < elmNum; i++) {
			AttrTableElement const& elm = table[i];

			rb_define_attr(klassID, elm.name, elm.read, elm.write);
			// std::cout << elm.name << std::endl;
		}
	}

	namespace ARuby
	{
		namespace
		{
			////////////////////////////////////////////////////////////
			/// Global variables
			////////////////////////////////////////////////////////////
			VALUE protected_objects;

			std::multimap< std::string, std::string > scripts_;
			std::string getScript(std::string const& name)
			{
				if( System::getScriptsPath().substr( System::getScriptsPath().find_last_of('.') + 1 ) == "rb" ) {
					assert( FileIO::exists(name) );
					std::vector< uint8_t > const& file = FileIO::get(name);
					return std::string( reinterpret_cast< char const* >( &(file[0]) ), file.size() );
				} else {
					assert( scripts_.find(name) != scripts_.end() );
					return scripts_.find(name)->second;
				}
			}
		} // namespace

		////////////////////////////////////////////////////////////
		/// ARGSS Ruby functions
		////////////////////////////////////////////////////////////
		VALUE rload_data(VALUE self, VALUE filename)
		{
			std::vector< uint8_t > const& data = FileFinder::FindFile( StringValueCStr(filename) );
			return rb_marshal_load( rb_str_new( reinterpret_cast< char const* >( &(data[0]) ), data.size() ) );
		}
		VALUE rsave_data(VALUE self, VALUE obj, VALUE filename)
		{
			VALUE file = rb_file_open(StringValueCStr(filename), "wb");
			rb_marshal_dump(obj, file);
			rb_io_close(file);
			return Qnil;
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Ruby initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			ruby_init();
			ruby_init_loadpath();
			atexit(ruby_finalize);

			protected_objects = rb_hash_new();
			rb_gc_register_address(&protected_objects);

			rb_define_global_function(ARGSS_FUNC(load_data), 1);
			rb_define_global_function(ARGSS_FUNC(save_data), 2);
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Ruby run
		////////////////////////////////////////////////////////////
		namespace
		{
			void checkError(int error)
			{
				if (error) {
					VALUE lasterr = rb_gv_get("$!");
					VALUE klass = rb_class_path(CLASS_OF(lasterr));
					VALUE message = rb_obj_as_string(lasterr);
					if (CLASS_OF(lasterr) != rb_eSystemExit) {
						std::string report = std::string("RUBY ERROR\n")
							+ RSTRING_PTR(klass) + " - " + RSTRING_PTR(message);
						if (!NIL_P(ruby_errinfo)) {
							VALUE ary = rb_funcall(ruby_errinfo, rb_intern("backtrace"), 0);
							for (int i = 0; i < RARRAY_LEN(ary); i++) {
								std::string errMsg( RSTRING_PTR(RARRAY_PTR(ary)[i]), RSTRING_LEN(RARRAY_PTR(ary)[i]) );

								report += "\n\tfrom " + errMsg + ":";
								if( i == ( RARRAY_LEN(ary) - 1 ) ) continue;

								boost::xpressive::sregex rex = boost::xpressive::sregex::compile("([^:]+):(\\d+):in `(.+)'");
								boost::xpressive::smatch what;
								assert( boost::xpressive::regex_match( errMsg, what, rex ) );
								std::string scriptName = what[1];
								unsigned int lineNo = boost::lexical_cast< unsigned int >( what[2] );

								std::istringstream iss( getScript(scriptName) );
								// iss.exceptions( std::ios_base::failbit | std::ios_base::badbit );
								int j = 0; std::string line;

								// skip lines
								for(int j_len = lineNo - BACK_TRACE_LINE_NUM; j < j_len; j++) std::getline(iss, line);

								for(int j_len = lineNo + BACK_TRACE_LINE_NUM; j < j_len; j++) {
									std::getline(iss, line);
									report += ( ( (j + 1) == lineNo ) ? "\n>" : "\n" ) + ( boost::format("%5d:") % (j + 1) ).str() + line;
									if( iss.eof() ) break;
								}
							}
						}
						Output::ErrorStr(report);
						return;
					}
				}
			}
			VALUE eval_wrap(VALUE arg)
			{
				return rb_funcall(
					rb_cObject, rb_intern("eval"), 4,
					rb_ary_entry(arg, 0),
					Qnil,
					rb_ary_entry(arg, 1), INT2NUM(1)
				);
			}
			VALUE require_wrap(VALUE arg)
			{
				std::string const& script = System::getScriptsPath();
				std::vector< uint8_t > const& data = FileFinder::FindFile(script);
				return rb_funcall(
					rb_cObject, rb_intern("eval"), 4,
					rb_str_new( reinterpret_cast< char const* >( &(data[0]) ), data.size() ),
					Qnil,
					rb_str_new( script.data(), script.size() ), INT2NUM(1)
				);
				// return rb_require( FileIO::toSlash(System::getScriptsPath()).c_str() );
			}
		} // namespace

		void Run()
		{
			int error;
			VALUE result;

			// current directory is excluded from load path at 1.9.2
			rb_eval_string("$: << File.dirname('.')");

			std::string
				scriptName = System::getScriptsPath(),
				suffix = scriptName.substr( scriptName.find_last_of('.') + 1 );

			if( (suffix == "rxdata") || (suffix == "rvdata") ) {
				VALUE scripts = rload_data( Qnil, rb_str_new( scriptName.data(), scriptName.size() ) );
				RArray* arr = RARRAY(scripts);
				for (int i = 0; i < RARRAY_LEN(arr); i++) {
					VALUE section_arr = rb_ary_entry(scripts, i);

					/*
					 * using boost.iostreams
					 * TODO: more shorter
					 */
					VALUE srcVal = rb_ary_entry(section_arr, 2);
					std::istringstream src(
						std::string( RSTRING_PTR(srcVal), RSTRING_LEN(srcVal) ),
						std::ios_base::in | std::ios_base::binary
					);
					std::ostringstream dst(std::ios_base::out | std::ios_base::binary);
					boost::iostreams::filtering_streambuf< boost::iostreams::input > in;
					in.push( boost::iostreams::zlib_decompressor() );
					in.push(src);
					boost::iostreams::copy(in, dst);
					std::string dstStr = dst.str();
					VALUE section = rb_str_new( dstStr.c_str(), dstStr.size() );

					std::string scriptName = RSTRING_PTR( rb_ary_entry(section_arr, 1) );
					scripts_.insert( std::multimap< std::string, std::string >::value_type( scriptName, dstStr ) );

					result = 
						// rb_eval_string_protect( RSTRING_PTR(section), &error );
						rb_protect(eval_wrap, rb_ary_new3(2, section, rb_ary_entry(section_arr, 1)), &error);
					checkError(error);
				}
			} else { // if( suffix == "rb" )
				result = rb_protect(require_wrap, Qundef, &error);
			}
			checkError(error);

			Player::Exit();
		}

		////////////////////////////////////////////////////////////
		/// Add a ruby object to the protected list
		////////////////////////////////////////////////////////////
		void AddObject(VALUE id)
		{
			rb_hash_aset(protected_objects, id, id);
		}

		////////////////////////////////////////////////////////////
		/// Remove a ruby object from the protected list
		////////////////////////////////////////////////////////////
		void RemoveObject(VALUE id) {
			rb_hash_delete(protected_objects, id);
		}
	} // namespace ARGSS
} // namespace ARuby

////////////////////////////////////////////////////////////
/// Check if object o is kind of c
////////////////////////////////////////////////////////////
void Check_Kind(VALUE o, VALUE c)
{
	if (!rb_obj_is_kind_of(o, c)) {
		rb_raise(rb_eTypeError, "wrong argument type %s (expected %s)", rb_class2name(o), rb_class2name(c));
	};
}

static struct types {
	int type;
	const char *name;
} builtin_types[] = {
	{T_NIL, "nil"},{T_OBJECT, "Object"},{T_CLASS, "Class"},{T_ICLASS, "iClass"},{T_MODULE, "Module"},
	{T_FLOAT, "Float"},{T_STRING, "String"},{T_REGEXP, "Regexp"},{T_ARRAY, "Array"},{T_FIXNUM, "Fixnum"},
	{T_HASH, "Hash"},{T_STRUCT, "Struct"},{T_BIGNUM, "Bignum"},{T_FILE, "File"},{T_TRUE, "true"},
	{T_FALSE, "false"},{T_SYMBOL, "Symbol"},{T_DATA, "Data"},{T_MATCH, "MatchData"},
#ifdef RUBY_VERSION_1_8
	{T_VARMAP, "Varmap"}, {T_SCOPE, "Scope"},
#endif
	{T_NODE, "Node"},{T_UNDEF, "undef"},{-1, 0}
};

////////////////////////////////////////////////////////////
/// Check if object x is type of t1 or t2
////////////////////////////////////////////////////////////
void Check_Types2(VALUE x, VALUE t1, VALUE t2)
{
	struct types *type = builtin_types;
	if (x == Qundef) rb_bug("undef leaked to the Ruby space");
	if ((VALUE)TYPE(x) != t1 && (VALUE)TYPE(x) != t2) {
		while (type->type >= 0) {
			if ((VALUE)type->type == t1) {
				char const* etype =
					NIL_P(x) ? "nil" :
					FIXNUM_P(x) ? "Fixnum" :
					SYMBOL_P(x) ? "Symbol" :
					rb_special_const_p(x) ? RSTRING_PTR(rb_obj_as_string(x)) :
					rb_class2name(x)
				;
				rb_raise(rb_eTypeError, "wrong argument type %s (expected %s)", etype, type->name);
			}
			type++;
		}
		rb_bug("unknown type 0x%x", (uint)t1);
	}
}

////////////////////////////////////////////////////////////
/// Check if object x is a boolean
////////////////////////////////////////////////////////////
void Check_Bool(VALUE x)
{
	if (x == Qundef) rb_bug("undef leaked to the Ruby space");
	if (TYPE(x) != T_TRUE && TYPE(x) != T_FALSE) {
		char const* etype =
			NIL_P(x) ? "nil" :
			FIXNUM_P(x) ? "Fixnum" :
			SYMBOL_P(x) ? "Symbol" :
			rb_special_const_p(x) ? RSTRING_PTR(rb_obj_as_string(x)) :
			rb_class2name(x)
		;
		rb_raise(rb_eTypeError, "wrong argument type %s (expected boolean)", etype);
	}
}

////////////////////////////////////////////////////////////
/// Check if object x class is c
////////////////////////////////////////////////////////////
void Check_Class(VALUE x, VALUE c)
{
	if (x == Qundef) {rb_bug("undef leaked to the Ruby space");}
	if (rb_class_real(CLASS_OF(x)) != c) {
		struct types *type = builtin_types;
		while (type->type >= 0) {
			if (type->type == TYPE(c)) {
				char const* etype =
					NIL_P(x) ? "nil" :
					FIXNUM_P(x) ? "Fixnum" :
					SYMBOL_P(x) ? "Symbol" :
					rb_special_const_p(x) ? RSTRING_PTR(rb_obj_as_string(x)) :
					rb_class2name(x)
				;
				rb_raise(rb_eTypeError, "wrong argument type %s (expected %s)", etype, type->name);
			}
			type++;
		}
		rb_bug("unknown type 0x%x", (uint)c);
	}
}

////////////////////////////////////////////////////////////
/// Check if object x is nil or its class is c
////////////////////////////////////////////////////////////
void Check_Classes_N(VALUE x, VALUE c)
{
	if ( NIL_P(x) ) return;
	else Check_Class(x, c);
}
