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
#include <string>

#include "argss_rect.hxx"


namespace ARGSS
{
	namespace ARect
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
		/// ARGSS Rect ruby functions
		////////////////////////////////////////////////////////////
		VALUE rinitialize(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h) {
			Check_Kind(x, rb_cNumeric);
			Check_Kind(y, rb_cNumeric);
			Check_Kind(w, rb_cNumeric);
			Check_Kind(h, rb_cNumeric);
			rb_iv_set(self, "@x", x);
			rb_iv_set(self, "@y", y);
			rb_iv_set(self, "@width", w);
			rb_iv_set(self, "@height", h);
			return self;
		}
		VALUE rset(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h) {
			Check_Kind(x, rb_cNumeric);
			Check_Kind(y, rb_cNumeric);
			Check_Kind(w, rb_cNumeric);
			Check_Kind(h, rb_cNumeric);
			rb_iv_set(self, "@x", x);
			rb_iv_set(self, "@y", y);
			rb_iv_set(self, "@width", w);
			rb_iv_set(self, "@height", h);
			return self;
		}
		VALUE rx(VALUE self) {
			return rb_iv_get(self, "@x");
		}
		VALUE rxE(VALUE self, VALUE x) {
			Check_Kind(x, rb_cNumeric);
			return rb_iv_set(self, "@x", x);
		}
		VALUE ry(VALUE self) {
			return rb_iv_get(self, "@y");
		}
		VALUE ryE(VALUE self, VALUE y) {
			Check_Kind(y, rb_cNumeric);
			return rb_iv_set(self, "@y", y);
		}
		VALUE rwidth(VALUE self) {
			return rb_iv_get(self, "@width");
		}
		VALUE rwidthE(VALUE self, VALUE w) {
			Check_Kind(w, rb_cNumeric);
			return rb_iv_set(self, "@width", w);
		}
		VALUE rheight(VALUE self) {
			return rb_iv_get(self, "@height");
		}
		VALUE rheightE(VALUE self, VALUE h) {
			Check_Kind(h, rb_cNumeric);
			return rb_iv_set(self, "@height", h);
		}
		VALUE rempty(VALUE self) {
			rb_iv_set(self, "@x", INT2NUM(0));
			rb_iv_set(self, "@y", INT2NUM(0));
			rb_iv_set(self, "@width", INT2NUM(0));
			rb_iv_set(self, "@height", INT2NUM(0));
			return self;
		}
		VALUE rinspect(VALUE self) {
			std::string str = (boost::format("(%d, %d, %d, %d)")
				% NUM2INT(rb_iv_get(self, "@x"))
				% NUM2INT(rb_iv_get(self, "@y"))
				% NUM2INT(rb_iv_get(self, "@width" ))
				% NUM2INT(rb_iv_get(self, "@height"))
			).str();
			return rb_str_new( str.c_str(), str.length() );
		}
		VALUE rdump(int argc, VALUE* argv, VALUE self) {
			if (argc > 1) raise_argn(argc, 1);
			VALUE arr = rb_ary_new3(4, rb_iv_get(self, "@x"), rb_iv_get(self, "@y"), rb_iv_get(self, "@width"), rb_iv_get(self, "@height"));
			return rb_funcall(arr, rb_intern("pack"), 1, rb_str_new2("l4"));
		}
		VALUE rload(VALUE self, VALUE str) {
			VALUE arr = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("l4"));
			VALUE args[4] = {rb_ary_entry(arr, 0), rb_ary_entry(arr, 1), rb_ary_entry(arr, 2), rb_ary_entry(arr, 3)};
			VALUE rect = rb_class_new_instance(4, args, id);
			return rect;
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Rect initialize
		////////////////////////////////////////////////////////////
		void Init() {
			id = rb_define_class("Rect", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), 4 },
				{ ARGSS_FUNC(set), 4 },
				ARGSS_GETTER_SETTER(x),
				ARGSS_GETTER_SETTER(y),
				ARGSS_GETTER_SETTER(width),
				ARGSS_GETTER_SETTER(height),
				{ ARGSS_FUNC(empty), 0 },
				{ ARGSS_FUNC(inspect), 0 },
				{ "_dump", RubyFunc(rdump), -1 },
			};
			defineMethods(id, funcTable);
			rb_define_singleton_method(id, "_load", RubyFunc(rload), 1);
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Rect new instance
		////////////////////////////////////////////////////////////
		VALUE New(double x, double y, double width, double height) {
			VALUE args[4] = {rb_float_new(x), rb_float_new(y), rb_float_new(width), rb_float_new(height)};
			return rb_class_new_instance(4, args, id);
		}
	} // namespace ARect
} // namespace ARGSS
