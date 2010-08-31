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

#include <argss/bitmap.hxx>
#include <argss/color.hxx>
#include <argss/error.hxx>
#include <argss/rect.hxx>
#include <argss/viewport.hxx>
#include <argss/window.hxx>

#include "../../VX/window.hxx"

////////////////////////////////////////////////////////////
/// Cap opacity value between 0 and 255
////////////////////////////////////////////////////////////
int CapValue(int v, int min = 0, int max = 255)
{
	return (v > max) ? max : (v < min) ? min : v;
}

namespace ARGSS
{
	namespace AWindow
	{
		namespace
		{
			////////////////////////////////////////////////////////////
			/// Global Variables
			////////////////////////////////////////////////////////////
			VALUE id;
		} // namespace

		////////////////////////////////////////////////////////////
		/// ARGSS Window ruby functions
		////////////////////////////////////////////////////////////
		VALUE rinitialize(int argc, VALUE *argv, VALUE self)
		{
			if (argc == 1) {
				Check_Classes_N(argv[0], ARGSS::AViewport::getID());
				rb_iv_set(self, "@viewport", argv[0]);
			}
			else if (argc == 0) {
				rb_iv_set(self, "@viewport", Qnil);
			}
			else raise_argn(argc, 1);
			rb_iv_set(self, "@windowskin", Qnil);
			rb_iv_set(self, "@contents", Qnil);
			rb_iv_set(self, "@stretch", Qtrue);
			rb_iv_set(self, "@cursor_rect", ARGSS::ARect::New(0, 0, 0, 0));
			rb_iv_set(self, "@active", Qtrue);
			rb_iv_set(self, "@visible", Qtrue);
			rb_iv_set(self, "@pause", Qfalse);
			rb_iv_set(self, "@x", INT2NUM(0));
			rb_iv_set(self, "@y", INT2NUM(0));
			rb_iv_set(self, "@width", INT2NUM(0));
			rb_iv_set(self, "@height", INT2NUM(0));
			rb_iv_set(self, "@z", INT2NUM(0));
			rb_iv_set(self, "@ox", INT2NUM(0));
			rb_iv_set(self, "@oy", INT2NUM(0));
			rb_iv_set(self, "@opacity", INT2NUM(255));
			rb_iv_set(self, "@back_opacity", INT2NUM(255));
			rb_iv_set(self, "@contents_opacity", INT2NUM(255));
			Window::New(self);
			ARGSS::ARuby::AddObject(self);
			return self;
		}
		VALUE rdispose(VALUE self)
		{
			if (!Window::IsDisposed(self)) {
				CheckDisposed(self);
				Window::Dispose(self);
				ARGSS::ARuby::RemoveObject(self);
				rb_gc();
			}
			return self;
		}
		VALUE rdisposedQ(VALUE self)
		{
			return INT2BOOL(Window::IsDisposed(self));
		}
		VALUE rupdate(VALUE self)
		{
			CheckDisposed(self);
			Window::get(self).Update();
			return Qnil;
		}
		VALUE rviewport(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@viewport");
		}
		VALUE rviewportE(VALUE self, VALUE viewport)
		{
			CheckDisposed(self);
			Check_Classes_N(viewport, ARGSS::AViewport::getID());
			Window::get(self).setViewport(viewport);
			return rb_iv_set(self, "@viewport", viewport);
		}
		VALUE rwindowskin(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@windowskin");
		}
		VALUE rwindowskinE(VALUE self, VALUE windowskin)
		{
			CheckDisposed(self);
			Check_Classes_N(windowskin, ARGSS::ABitmap::getID());
			Window::get(self).setWindowskin(windowskin);
			return rb_iv_set(self, "@windowskin", windowskin);
		}
		VALUE rcontents(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@contents");
		}
		VALUE rcontentsE(VALUE self, VALUE contents)
		{
			CheckDisposed(self);
			Check_Classes_N(contents, ARGSS::ABitmap::getID());
			Window::get(self).setContents(contents);
			return rb_iv_set(self, "@contents", contents);
		}
		VALUE rstretch(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@stretch");
		}
		VALUE rstretchE(VALUE self, VALUE stretch)
		{
			CheckDisposed(self);
			Window::get(self).setStretch(NUM2BOOL(stretch));
			return rb_iv_set(self, "@stretch", stretch);
		}
		VALUE rcursor_rect(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@cursor_rect");
		}
		VALUE rcursor_rectE(VALUE self, VALUE cursor_rect)
		{
			CheckDisposed(self);
			Check_Class(cursor_rect, ARGSS::ARect::getID());
			Window::get(self).setCursorRect(cursor_rect);
			return rb_iv_set(self, "@cursor_rect", cursor_rect);
		}
		VALUE ractive(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@active");
		}
		VALUE ractiveE(VALUE self, VALUE active)
		{
			CheckDisposed(self);
			Window::get(self).setActive(NUM2BOOL(active));
			return rb_iv_set(self, "@active", active);
		}
		VALUE rvisible(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@visible");
		}
		VALUE rvisibleE(VALUE self, VALUE visible)
		{
			CheckDisposed(self);
			Window::get(self).setVisible(NUM2BOOL(visible));
			return rb_iv_set(self, "@visible", visible);
		}
		VALUE rpause(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@pause");
		}
		VALUE rpauseE(VALUE self, VALUE pause)
		{
			CheckDisposed(self);
			Window::get(self).setPause(NUM2BOOL(pause));
			return rb_iv_set(self, "@pause", pause);
		}
		VALUE rx(VALUE self)
		{
			CheckDisposed(self);
			return rb_to_int(rb_iv_get(self, "@x"));
		}
		VALUE rfx(VALUE self)
		{
			CheckDisposed(self);
			return rb_Float(rb_iv_get(self, "@x"));
		}
		VALUE rxE(VALUE self, VALUE x)
		{
			CheckDisposed(self);
			Window::get(self).setX(NUM2INT(x));
			return rb_iv_set(self, "@x", x);
		}
		VALUE ry(VALUE self)
		{
			CheckDisposed(self);
			return rb_to_int(rb_iv_get(self, "@y"));
		}
		VALUE rfy(VALUE self)
		{
			CheckDisposed(self);
			return rb_Float(rb_iv_get(self, "@y"));
		}
		VALUE ryE(VALUE self, VALUE y)
		{
			CheckDisposed(self);
			Window::get(self).setY(NUM2INT(y));
			return rb_iv_set(self, "@y", y);
		}
		VALUE rwidth(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@width");
		}
		VALUE rwidthE(VALUE self, VALUE width)
		{
			CheckDisposed(self);
			int w = NUM2INT(width);
			if (w < 0) w = 0;
			Window::get(self).setWidth(w);
			return rb_iv_set(self, "@width", INT2NUM(w));
		}
		VALUE rheight(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@height");
		}
		VALUE rheightE(VALUE self, VALUE height)
		{
			CheckDisposed(self);
			int h = NUM2INT(height);
			if (h < 0) h = 0;
			Window::get(self).setHeight(h);
			return rb_iv_set(self, "@height", INT2NUM(h));
		}
		VALUE rz(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@z");
		}
		VALUE rzE(VALUE self, VALUE z)
		{
			CheckDisposed(self);
			Window::get(self).setZ(NUM2INT(z));
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
			Window::get(self).setOx(NUM2INT(ox));
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
			Window::get(self).setOy(NUM2INT(oy));
			return rb_iv_set(self, "@oy", oy);
		}
		VALUE ropacity(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@opacity");
		}
		VALUE ropacityE(VALUE self, VALUE opacity)
		{
			CheckDisposed(self);
			opacity = CapValue(NUM2INT(opacity));
			Window::get(self).setOpacity(opacity);
			return rb_iv_set(self, "@opacity", INT2NUM(opacity));
		}
		VALUE rback_opacity(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@back_opacity");
		}
		VALUE rback_opacityE(VALUE self, VALUE back_opacity)
		{
			CheckDisposed(self);
			back_opacity = CapValue(NUM2INT(back_opacity));
			Window::get(self).setBackOpacity(back_opacity);
			return rb_iv_set(self, "@back_opacity", INT2NUM(back_opacity));
		}
		VALUE rcontents_opacity(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@contents_opacity");
		}
		VALUE rcontents_opacityE(VALUE self, VALUE contents_opacity)
		{
			CheckDisposed(self);
			contents_opacity = CapValue(NUM2INT(contents_opacity));
			Window::get(self).setContentsOpacity(contents_opacity);
			return rb_iv_set(self, "@contents_opacity", INT2NUM(contents_opacity));
		}
		VALUE ropenness(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@openness");
		}
		VALUE ropennessE(VALUE self, VALUE openness)
		{
			CheckDisposed(self);
			openness = CapValue(NUM2INT(openness));
			Window::get(self).setOpenness(openness);
			return rb_iv_set(self, "@openness", INT2NUM(openness));
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Window initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_class("Window", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), -1 },
				{ ARGSS_FUNC(dispose), 0 }, { ARGSS_Q(disposed), 0 },
				{ ARGSS_FUNC(update), 0 },
				ARGSS_GETTER_SETTER(viewport),
				ARGSS_GETTER_SETTER(windowskin),
				ARGSS_GETTER_SETTER(contents),
				ARGSS_GETTER_SETTER(stretch),
				ARGSS_GETTER_SETTER(cursor_rect),
				ARGSS_GETTER_SETTER(active),
				{ "visible?", RubyFunc(rvisible), 0 },
				ARGSS_GETTER_SETTER(visible),
				ARGSS_GETTER_SETTER(pause),
				ARGSS_GETTER_SETTER(x), { "fx", RubyFunc(rx), 0 },
				ARGSS_GETTER_SETTER(y), { "fy", RubyFunc(ry), 0 },
				ARGSS_GETTER_SETTER(width),
				ARGSS_GETTER_SETTER(height),
				ARGSS_GETTER_SETTER(z),
				ARGSS_GETTER_SETTER(ox),
				ARGSS_GETTER_SETTER(oy),
				ARGSS_GETTER_SETTER(opacity),
				ARGSS_GETTER_SETTER(back_opacity),
				ARGSS_GETTER_SETTER(contents_opacity),
				ARGSS_GETTER_SETTER(openness),
			};
			defineMethods(id, funcTable);
		}

		////////////////////////////////////////////////////////////
		/// Check if window isn't disposed
		////////////////////////////////////////////////////////////
		void CheckDisposed(VALUE self)
		{
			if (Window::IsDisposed(self)) {
				rb_raise(ARGSS::AError::getID(), "disposed window <%i>", (int)self);
			}
		}
	} // namespace AWindow
} // namespace ARGSS
