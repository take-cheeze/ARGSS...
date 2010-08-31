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

#include <argss/color.hxx>
#include <argss/error.hxx>
#include <argss/rect.hxx>
#include <argss/tone.hxx>
#include <argss/viewport.hxx>

#include <viewport.hxx>

namespace ARGSS
{
	namespace AViewport
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
		/// ARGSS Viewport ruby functions
		////////////////////////////////////////////////////////////
		VALUE rinitialize(int argc, VALUE *argv, VALUE self)
		{
			if (argc == 0) raise_argn(argc, 1);
			else if (argc > 1 && argc < 4) raise_argn(argc, 4);
			if (argc == 1) rb_iv_set(self, "@rect", argv[0]);
			else rb_iv_set(self, "@rect", ARGSS::ARect::New(argv[0], argv[1], argv[2], argv[3]));
			rb_iv_set(self, "@visible", Qtrue);
			rb_iv_set(self, "@z", INT2NUM(0));
			rb_iv_set(self, "@ox", INT2NUM(0));
			rb_iv_set(self, "@oy", INT2NUM(0));
			rb_iv_set(self, "@color", ARGSS::AColor::New(0, 0, 0, 0));
			rb_iv_set(self, "@tone", ARGSS::ATone::New());
			Viewport::New(self);
			ARGSS::ARuby::AddObject(self);
			return self;
		}
		VALUE rdispose(VALUE self)
		{
			if (!Viewport::IsDisposed(self)) {
				CheckDisposed(self);
				Viewport::Dispose(self);
				ARGSS::ARuby::RemoveObject(self);
				rb_gc();
			}
			return self;
		}
		VALUE rdisposedQ(VALUE self)
		{
			return INT2BOOL(Viewport::IsDisposed(self));
		}
		VALUE rflash(VALUE self, VALUE color, VALUE duration)
		{
			CheckDisposed(self);
			if ( NIL_P(color) ) Viewport::get(self).Flash(NUM2INT(duration));
			else Viewport::get(self).Flash(Color(color), NUM2INT(duration));
			return Qnil;
		}
		VALUE rupdate(VALUE self)
		{
			CheckDisposed(self);
			Viewport::get(self).Update();
			return Qnil;
		}
		VALUE rrect(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@rect");
		}
		VALUE rrectE(VALUE self, VALUE rect)
		{
			CheckDisposed(self);
			Check_Class(rect, ARGSS::ARect::getID());
			Viewport::get(self).setRect(rect);
			return rb_iv_set(self, "@rect", rect);
		}
		VALUE rvisible(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@visible");
		}
		VALUE rvisibleE(VALUE self, VALUE visible)
		{
			CheckDisposed(self);
			Viewport::get(self).setVisible(NUM2BOOL(visible));
			return rb_iv_set(self, "@visible", visible);
		}
		VALUE rz(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@z");
		}
		VALUE rzE(VALUE self, VALUE z)
		{
			CheckDisposed(self);
			Viewport::get(self).setZ(NUM2INT(z));
			return rb_iv_set(self, "@z", z);
		}
		VALUE rox(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@ox");
		}
		VALUE roxE(VALUE self, VALUE ox)
		{
			CheckDisposed(self);
			Viewport::get(self).setOx(NUM2INT(ox));
			return rb_iv_set(self, "@ox", ox);
		}
		VALUE roy(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@oy");
		}
		VALUE royE(VALUE self, VALUE oy)
		{
			CheckDisposed(self);
			Viewport::get(self).setOy(NUM2INT(oy));
			return rb_iv_set(self, "@oy", oy);
		}
		VALUE rcolor(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@color");
		}
		VALUE rcolorE(VALUE self, VALUE color)
		{
			CheckDisposed(self);
			Check_Class(color, ARGSS::AColor::getID());
			Viewport::get(self).setColor(color);
			return rb_iv_set(self, "@color", color);
		}
		VALUE rtone(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@tone");
		}
		VALUE rtoneE(VALUE self, VALUE tone)
		{
			CheckDisposed(self);
			Check_Class(tone, ARGSS::ATone::getID());
			Viewport::get(self).setTone(tone);
			return rb_iv_set(self, "@tone", tone);
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Viewport initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_class("Viewport", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), -1 },
				{ ARGSS_FUNC(dispose), 0 }, { ARGSS_Q(disposed), 0 },
				{ ARGSS_FUNC(flash), 2 },
				{ ARGSS_FUNC(update), 0 },
				ARGSS_GETTER_SETTER(rect),
				{ "visible?", RubyFunc(rvisible), 0 },
				ARGSS_GETTER_SETTER(visible),
				ARGSS_GETTER_SETTER(z),
				ARGSS_GETTER_SETTER(ox),
				ARGSS_GETTER_SETTER(oy),
				ARGSS_GETTER_SETTER(color),
				ARGSS_GETTER_SETTER(tone),
			};
			defineMethods(id, funcTable);
		}

		////////////////////////////////////////////////////////////
		/// Check if plane isn't disposed
		////////////////////////////////////////////////////////////
		void CheckDisposed(VALUE self) {
			if (Viewport::IsDisposed(self)) {
				rb_raise(ARGSS::AError::getID(), "disposed viewport <%i>", (int)self);
			}
		}
	} // namespace AViewport
} // namespace ARGSS
