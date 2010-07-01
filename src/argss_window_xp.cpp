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
#include "argss_window_xp.h"
#include "argss_viewport.h"
#include "argss_bitmap.h"
#include "argss_color.h"
#include "argss_rect.h"
#include "argss_error.h"
#include "window_xp.h"

////////////////////////////////////////////////////////////
/// Cap opacity value between 0 and 255
////////////////////////////////////////////////////////////
int CapOpacityValue(int v)
{
	return (v > 255) ? 255 : (v < 0) ? 0 : v;
}

namespace ARGSS
{
	namespace AWindow
	{
		////////////////////////////////////////////////////////////
		/// Global Variables
		////////////////////////////////////////////////////////////
		VALUE id;

		////////////////////////////////////////////////////////////
		/// ARGSS Window ruby functions
		////////////////////////////////////////////////////////////
		VALUE rinitialize(int argc, VALUE *argv, VALUE self)
		{
			if (argc == 1) {
				Check_Classes_N(argv[0], ARGSS::AViewport::id);
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
				rb_gc_start();
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
			Window::Get(self)->Update();
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
			Check_Classes_N(viewport, ARGSS::AViewport::id);
			Window::Get(self)->SetViewport(viewport);
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
			Check_Classes_N(windowskin, ARGSS::ABitmap::id);
			Window::Get(self)->SetWindowskin(windowskin);
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
			Check_Classes_N(contents, ARGSS::ABitmap::id);
			Window::Get(self)->SetContents(contents);
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
			Window::Get(self)->SetStretch(NUM2BOOL(stretch));
			return rb_iv_set(self, "@stretch", stretch);
		}
		VALUE rcursor_rect(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@cursor_rect");
		}
		VALUE rcursor_rectE(VALUE self, VALUE cursor_rect)
		{
			CheckDisposed(self);
			Check_Class(cursor_rect, ARGSS::ARect::id);
			Window::Get(self)->SetCursorRect(cursor_rect);
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
			Window::Get(self)->SetActive(NUM2BOOL(active));
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
			Window::Get(self)->SetVisible(NUM2BOOL(visible));
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
			Window::Get(self)->SetPause(NUM2BOOL(pause));
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
			Window::Get(self)->SetX(NUM2INT(x));
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
			Window::Get(self)->SetY(NUM2INT(y));
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
			Window::Get(self)->SetWidth(w);
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
			Window::Get(self)->SetHeight(h);
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
			Window::Get(self)->SetZ(NUM2INT(z));
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
			Window::Get(self)->SetOx(NUM2INT(ox));
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
			Window::Get(self)->SetOy(NUM2INT(oy));
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
			opacity = CapOpacityValue(NUM2INT(opacity));
			Window::Get(self)->SetOpacity(opacity);
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
			back_opacity = CapOpacityValue(NUM2INT(back_opacity));
			Window::Get(self)->SetBackOpacity(back_opacity);
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
			contents_opacity = CapOpacityValue(NUM2INT(contents_opacity));
			Window::Get(self)->SetContentsOpacity(contents_opacity);
			return rb_iv_set(self, "@contents_opacity", INT2NUM(contents_opacity));
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Window initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_class("Window", rb_cObject);
			rb_define_method(id, "initialize", RubyFunc(rinitialize), -1);
			rb_define_method(id, "dispose", RubyFunc(rdispose), 0);
			rb_define_method(id, "disposed?", RubyFunc(rdisposedQ), 0);
			rb_define_method(id, "update", RubyFunc(rupdate), 0);
			rb_define_method(id, "viewport", RubyFunc(rviewport), 0);
			rb_define_method(id, "viewport=", RubyFunc(rviewportE), 1);
			rb_define_method(id, "windowskin", RubyFunc(rwindowskin), 0);
			rb_define_method(id, "windowskin=", RubyFunc(rwindowskinE), 1);
			rb_define_method(id, "contents", RubyFunc(rcontents), 0);
			rb_define_method(id, "contents=", RubyFunc(rcontentsE), 1);
			rb_define_method(id, "stretch", RubyFunc(rstretch), 0);
			rb_define_method(id, "stretch=", RubyFunc(rstretchE), 1);
			rb_define_method(id, "cursor_rect", RubyFunc(rcursor_rect), 0);
			rb_define_method(id, "cursor_rect=", RubyFunc(rcursor_rectE), 1);
			rb_define_method(id, "active", RubyFunc(ractive), 0);
			rb_define_method(id, "active=", RubyFunc(ractiveE), 1);
			rb_define_method(id, "visible", RubyFunc(rvisible), 0);
			rb_define_method(id, "visible=", RubyFunc(rvisibleE), 1);
			rb_define_method(id, "pause", RubyFunc(rpause), 0);
			rb_define_method(id, "pause=", RubyFunc(rpauseE), 1);
			rb_define_method(id, "x", RubyFunc(rx), 0);
			rb_define_method(id, "fx", RubyFunc(rx), 0);
			rb_define_method(id, "x=", RubyFunc(rxE), 1);
			rb_define_method(id, "y", RubyFunc(ry), 0);
			rb_define_method(id, "fy", RubyFunc(ry), 0);
			rb_define_method(id, "y=", RubyFunc(ryE), 1);
			rb_define_method(id, "width", RubyFunc(rwidth), 0);
			rb_define_method(id, "width=", RubyFunc(rwidthE), 1);
			rb_define_method(id, "height", RubyFunc(rheight), 0);
			rb_define_method(id, "height=", RubyFunc(rheightE), 1);
			rb_define_method(id, "z", RubyFunc(rz), 0);
			rb_define_method(id, "z=", RubyFunc(rzE), 1);
			rb_define_method(id, "ox", RubyFunc(rox), 0);
			rb_define_method(id, "ox=", RubyFunc(roxE), 1);
			rb_define_method(id, "oy", RubyFunc(roy), 0);
			rb_define_method(id, "oy=", RubyFunc(royE), 1);
			rb_define_method(id, "opacity", RubyFunc(ropacity), 0);
			rb_define_method(id, "opacity=", RubyFunc(ropacityE), 1);
			rb_define_method(id, "back_opacity", RubyFunc(rback_opacity), 0);
			rb_define_method(id, "back_opacity=", RubyFunc(rback_opacityE), 1);
			rb_define_method(id, "contents_opacity", RubyFunc(rcontents_opacity), 0);
			rb_define_method(id, "contents_opacity=", RubyFunc(rcontents_opacityE), 1);
		}

		////////////////////////////////////////////////////////////
		/// Check if window isn't disposed
		////////////////////////////////////////////////////////////
		void CheckDisposed(VALUE self)
		{
			if (Window::IsDisposed(self)) {
				rb_raise(ARGSS::AError::id, "disposed window <%i>", (int)self);
			}
		}
	} // namespace AWindow
} // namespace ARGSS
