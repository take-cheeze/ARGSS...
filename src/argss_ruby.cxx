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
#include <cassert>

#include <iomanip>
#include <iostream>
#include <string>

#include "argss_ruby.hxx"
#include "options.hxx"
#include "system.hxx"
#include "player.hxx"
#include "output.hxx"

#ifndef RUBY_VERSION_1_8
	#define ruby_errinfo rb_errinfo()
#endif

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

	namespace ARuby
	{
		////////////////////////////////////////////////////////////
		/// Global variables
		////////////////////////////////////////////////////////////
		VALUE protected_objects;

		////////////////////////////////////////////////////////////
		/// ARGSS Ruby functions
		////////////////////////////////////////////////////////////
		VALUE rload_data(VALUE self, VALUE filename)
		{
			VALUE file = rb_file_open(StringValuePtr(filename), "rb");
			VALUE obj = rb_marshal_load(file);
			rb_io_close(file);
			return obj;
		}
		VALUE rsave_data(VALUE self, VALUE obj, VALUE filename)
		{
			VALUE file = rb_file_open(StringValuePtr(filename), "wb");
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
			//atexit(ruby_finalize);

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
						std::string report = "RUBY ERROR\n";
						report += std::string( RSTRING_PTR(klass) );
						report += " - ";
						report += std::string( RSTRING_PTR(message) );
						if (!NIL_P(ruby_errinfo)) {
							VALUE ary = rb_funcall(ruby_errinfo, rb_intern("backtrace"), 0);
							for (int i = 0; i < RARRAY_LEN(ary); i++) {
								report += "\n  from ";
								report += RSTRING_PTR(RARRAY_PTR(ary)[i]);
							}
						}
						Output::ErrorStr(report);
						return;
					}
				}
			}
			std::string slasher(std::string const& str)
			{
				std::string ret;
				for(std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
					ret.push_back( (*it == '\\') ? '/' : *it );
				}
				return ret;
			}
			VALUE eval_wrap(VALUE arg)
			{
/*
				std::cout
					<< RSTRING_PTR( rb_ary_entry(arg, 1) ) << std::endl
					<< RSTRING_PTR( rb_ary_entry(arg, 0) ) << std::endl
				;
 */
				return rb_funcall(
					rb_cObject, rb_intern("eval"), 4,
					rb_ary_entry(arg, 0),
					Qnil,
					rb_ary_entry(arg, 1),
					INT2NUM(1)
				);
			}
			VALUE require_wrap(VALUE arg)
			{
				return rb_require( slasher(System::getScriptsPath()).c_str() );
			}
		} // namespace
		void Run()
		{
			int error;
			VALUE result;

			std::string
				target = slasher(System::getScriptsPath()),
				suffix = target.substr( target.find_last_of('.') + 1 );

			if( suffix == "rxdata" ) { // if (SCRIPTS_ZLIB) {
				int error;
				VALUE result = rb_require("zlib");
				VALUE cZlib = rb_const_get(rb_cObject, rb_intern("Zlib"));
				VALUE cInflate = rb_const_get(cZlib, rb_intern("Inflate"));
				VALUE file = rb_file_open(target.c_str(), "rb");
				VALUE scripts = rb_marshal_load(file);
				RArray* arr = RARRAY(scripts);
				for (int i = 0; i < RARRAY_LEN(arr); i++) {
					VALUE section_arr = rb_ary_entry(scripts, i);
					VALUE section = rb_funcall( cInflate, rb_intern("inflate"), 1, rb_ary_entry(section_arr, 2) );

/*
					std::cout
						<< std::setw(4) << i
							<< " : " << std::setw(8) << RSTRING_LEN(section)
							<< " : " << RSTRING_PTR( rb_ary_entry(section_arr, 1) )
						<< std::endl
						// << RSTRING_PTR(section) << std::endl
						;
 */

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
	if (x == Qnil) return;
	Check_Class(x, c);
}
