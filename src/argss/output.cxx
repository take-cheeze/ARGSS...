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
#include <string>

#include <argss/output.hxx>

#include <output.hxx>

namespace ARGSS
{
	namespace AOutput
	{
		namespace
		{
			////////////////////////////////////////////////////////////
			/// Global Variables
			////////////////////////////////////////////////////////////
			VALUE id;
			VALUE stdout_id;
			VALUE stderr_id;
			VALUE stdin_id;
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Output ruby functions
		////////////////////////////////////////////////////////////
		VALUE rconsole(VALUE self)
		{
			Output::Console();
			return Qnil;
		}
		VALUE rmsgbox(VALUE self)
		{
			Output::MsgBox();
			return Qnil;
		}
		VALUE rfile(VALUE self, VALUE file)
		{
			Output::File(StringValueCStr(file));
			return Qnil;
		}
		VALUE rnone(VALUE self)
		{
			Output::None();
			return Qnil;
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Stdout, Stderr and Stdin
		////////////////////////////////////////////////////////////
		VALUE rstdout_write(VALUE self, VALUE str)
		{
			if (TYPE(str) != T_STRING) str = rb_obj_as_string(str);
			if (RSTRING_LEN(str) == 0) return INT2FIX(0);
			Output::PostStr(StringValueCStr(str));
			return INT2FIX(RSTRING_LEN(str));
		}
		VALUE rstderr_write(VALUE self, VALUE str)
		{
			if (TYPE(str) != T_STRING) str = rb_obj_as_string(str);
			if (RSTRING_LEN(str) == 0) return INT2FIX(0);
			//Output::ErrorStr(StringValueCStr(str));
			return INT2FIX(RSTRING_LEN(str));
		}
		VALUE stdin_gets(int argc, VALUE *argv, VALUE self)
		{
			std::string str = Output::getString();
			return rb_str_new(str.c_str(), str.length());
		}
		VALUE stdin_getc(int argc, VALUE *argv, VALUE self)
		{
			std::string str = Output::getChar();
			return rb_str_new(str.c_str(), str.length());
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Output initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_module("Output");
			rb_define_singleton_method(id, ARGSS_FUNC(console), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(msgbox), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(file), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(none), 0);  

			stdout_id = rb_define_class_under(id, "Stdout", rb_cObject);
			rb_define_method(stdout_id, "write", RubyFunc(rstdout_write), 1);
			stderr_id = rb_define_class_under(id, "Stderr", rb_cObject);
			rb_define_method(stderr_id, "write", RubyFunc(rstderr_write), 1);

			rb_gv_set("$stdout", rb_class_new_instance(0, 0, stdout_id));
			rb_gv_set("$stderr", rb_class_new_instance(0, 0, stderr_id));

			stdin_id = rb_define_class_under(id, "Stdin", rb_cObject);
			rb_define_method(stdin_id, "gets", RubyFunc(stdin_gets), -1);
			rb_define_method(stdin_id, "getc", RubyFunc(stdin_getc), -1);
			rb_define_global_function("getc", RubyFunc(stdin_getc), 0);

			rb_gv_set("$stdin", rb_class_new_instance(0, 0, stdin_id));
		}
	} // namespace AOutput
} // namespace ARGSS
