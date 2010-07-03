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
#include "argss_font.hxx"
#include "argss_color.hxx"

namespace ARGSS
{
	namespace AFont
	{
		namespace
		{
			////////////////////////////////////////////////////////////
			/// Global Variables
			////////////////////////////////////////////////////////////
			VALUE id;
		} // namespace
		VALUE& getID() { return id; }

		////////////////////////////////////////////////////////////
		/// ARGSS Font ruby functions
		////////////////////////////////////////////////////////////
		VALUE rinitialize(int argc, VALUE *argv, VALUE self)
		{
			if (argc > 2) raise_argn(argc, 2);
			if (argc >= 1) {
				Check_Types2(argv[0], T_STRING, T_ARRAY);
				rb_iv_set(self, "@name", argv[0]);
			}
			else {
				rb_iv_set(self, "@name", rb_str_dup(rb_cv_get(id, "@@default_name")));
			}
			if (argc == 2) {
				Check_Kind(argv[1], rb_cNumeric);
				rb_iv_set(self, "@size", argv[1]);
			}
			else {
				rb_iv_set(self, "@size", rb_cv_get(id, "@@default_size"));
			}
			rb_iv_set(self, "@bold", rb_cv_get(id, "@@default_bold"));
			rb_iv_set(self, "@italic", rb_cv_get(id, "@@default_italic"));
			rb_iv_set(self, "@color", ARGSS::AColor::New(rb_cv_get(id, "@@default_color")));
			return self;
		}

		VALUE rexistQ(VALUE self)
		{
			// TODO
			return Qtrue;
		}
		VALUE rname(VALUE self)
		{
			return rb_iv_get(self, "@name");
		}
		VALUE rnameE(VALUE self, VALUE name)
		{
			Check_Types2(name, T_STRING, T_ARRAY);
			rb_iv_set(self, "@name", name);
			return name;
		}
		VALUE rsize(VALUE self)
		{
			return rb_iv_get(self, "@size");
		}
		VALUE rsizeE(VALUE self, VALUE size)
		{
			Check_Kind(size, rb_cNumeric);
			rb_iv_set(self, "@size", size);
			return size; 
		}
		VALUE rbold(VALUE self)
		{
			return rb_iv_get(self, "@bold");
		}
		VALUE rboldE(VALUE self, VALUE bold) {
			Check_Bool(bold);
			rb_iv_set(self, "@bold", bold);
			return bold;
		}
		VALUE ritalic(VALUE self)
		{
			return rb_iv_get(self, "@italic");
		}
		VALUE ritalicE(VALUE self, VALUE italic)
		{
			Check_Bool(italic);
			rb_iv_set(self, "@italic", italic);
			return italic;
		}
		VALUE rcolor(VALUE self)
		{
			return rb_iv_get(self, "@color");
		}
		VALUE rcolorE(VALUE self, VALUE color)
		{
			Check_Class( color, ARGSS::AColor::getID() );
			return rb_iv_set(self, "@color", color);
		}
		VALUE rshadow(VALUE self)
		{
			return rb_iv_get(self, "@shadow");
		}
		VALUE rshadowE(VALUE self, VALUE shadow)
		{
			Check_Bool(shadow);
			rb_iv_set(self, "@shadow", shadow);
			return shadow;
		}
		VALUE rdefault_name(VALUE self)
		{
			return rb_cv_get(id, "@@default_name");
		}
		VALUE rdefault_nameE(VALUE self, VALUE default_name)
		{
			Check_Types2(default_name, T_STRING, T_ARRAY);
			rb_cv_set(id, "@@default_name", default_name);
			return default_name;
		}
		VALUE rdefault_size(VALUE self)
		{
			return rb_cv_get(id, "@@default_size");
		}
		VALUE rdefault_sizeE(VALUE self, VALUE default_size) {
			Check_Kind(default_size, rb_cNumeric);
			rb_cv_set(id, "@@default_size", default_size);
			return default_size;
		}
		VALUE rdefault_bold(VALUE self) {
			return rb_cv_get(id, "@@default_bold");
		}
		VALUE rdefault_boldE(VALUE self, VALUE default_bold) {
			Check_Bool(default_bold);
			rb_cv_set(id, "@@default_bold", default_bold);
			return default_bold;
		}
		VALUE rdefault_italic(VALUE self) {
			return rb_cv_get(id, "@@default_italic");
		}
		VALUE rdefault_italicE(VALUE self, VALUE default_italic) {
			Check_Bool(default_italic);
			rb_cv_set(id, "@@default_italic", default_italic);
			return default_italic;
		}
		VALUE rdefault_color(VALUE self)
		{
			return rb_cv_get(id, "@@default_color");
		}
		VALUE rdefault_colorE(VALUE self, VALUE default_color)
		{
			Check_Class( default_color, ARGSS::AColor::getID() );
			rb_cv_set(id, "@@default_color", default_color);
			return default_color;
		}
		VALUE rdefault_shadow(VALUE self)
		{
			return rb_cv_get(id, "@@default_shadow");
		}
		VALUE rdefault_shadowE(VALUE self, VALUE default_shadow)
		{
			Check_Bool(default_shadow);
			rb_cv_set(id, "@@default_shadow", default_shadow);
			return default_shadow;
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Font initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_class("Font", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), -1 },
				{ ARGSS_Q(exist), 0 },
				ARGSS_GETTER_SETTER(name),
				ARGSS_GETTER_SETTER(size),
				ARGSS_GETTER_SETTER(bold),
				ARGSS_GETTER_SETTER(italic),
				ARGSS_GETTER_SETTER(color),
				ARGSS_GETTER_SETTER(shadow),
			};
			defineMethods(id, funcTable);
			rb_define_singleton_method(id, ARGSS_FUNC(default_name), 0);
			rb_define_singleton_method(id, ARGSS_E(default_name), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(default_size), 0);
			rb_define_singleton_method(id, ARGSS_E(default_size), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(default_bold), 0);
			rb_define_singleton_method(id, ARGSS_E(default_bold), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(default_italic), 0);
			rb_define_singleton_method(id, ARGSS_E(default_italic), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(default_color), 0);
			rb_define_singleton_method(id, ARGSS_E(default_color), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(default_shadow), 0);
			rb_define_singleton_method(id, ARGSS_E(default_shadow), 1);
			rb_cv_set(id, "@@default_name", rb_str_new2("Arial"));
			rb_cv_set(id, "@@default_size", INT2NUM(22));
			rb_cv_set(id, "@@default_bold", Qfalse);
			rb_cv_set(id, "@@default_italic", Qfalse);
			rb_cv_set(id, "@@default_color", ARGSS::AColor::New(255, 255, 255, 255));
			rb_cv_set(id, "@@default_shadow", Qfalse);
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Font new ruby instance
		////////////////////////////////////////////////////////////
		VALUE New()
		{
			return rb_class_new_instance(0, 0, id);
		}
	} // namespace AFont
} // namespace ARGSS
