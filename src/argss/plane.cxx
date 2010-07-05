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
#include <argss/plane.hxx>
#include <argss/tone.hxx>
#include <argss/viewport.hxx>

#include <plane.hxx>

namespace ARGSS
{
	namespace APlane
	{
		namespace
		{
			////////////////////////////////////////////////////////////
			/// Global Variables
			////////////////////////////////////////////////////////////
			VALUE id;
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Plane ruby functions
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
			rb_iv_set(self, "@bitmap", Qnil);
			rb_iv_set(self, "@visible", Qtrue);
			rb_iv_set(self, "@z", INT2NUM(0));
			rb_iv_set(self, "@ox", INT2NUM(0));
			rb_iv_set(self, "@oy", INT2NUM(0));
			rb_iv_set(self, "@zoom_x", rb_float_new(1.0f));
			rb_iv_set(self, "@zoom_y", rb_float_new(1.0f));
			rb_iv_set(self, "@opacity", INT2NUM(255));
			rb_iv_set(self, "@blend_type", INT2NUM(0));
			rb_iv_set(self, "@color", ARGSS::AColor::New());
			rb_iv_set(self, "@tone", ARGSS::ATone::New());
			Plane::New(self);
			ARGSS::ARuby::AddObject(self);
			return self;
		}
		VALUE rdispose(VALUE self)
		{
			if (!Plane::IsDisposed(self)) {
				CheckDisposed(self);
				Plane::Dispose(self);
				ARGSS::ARuby::RemoveObject(self);
				rb_gc_start();
			}
			return self;
		}
		VALUE rdisposedQ(VALUE self)
		{
			return INT2BOOL(Plane::IsDisposed(self));
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
			Plane::get(self).setViewport(viewport);
			return rb_iv_set(self, "@viewport", viewport);
		}
		VALUE rbitmap(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@bitmap");
		}
		VALUE rbitmapE(VALUE self, VALUE bitmap)
		{
			CheckDisposed(self);
			Check_Classes_N(bitmap, ARGSS::ABitmap::getID());
			Plane::get(self).setBitmap(bitmap);
			return rb_iv_set(self, "@bitmap", bitmap);
		}
		VALUE rvisible(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@visible");
		}
		VALUE rvisibleE(VALUE self, VALUE visible)
		{
			CheckDisposed(self);
			Plane::get(self).setVisible(NUM2BOOL(visible));
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
			Plane::get(self).setZ(NUM2INT(z));
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
			Plane::get(self).setOx(NUM2INT(ox));
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
			Plane::get(self).setOy(NUM2INT(oy));
			return rb_iv_set(self, "@oy", oy);
		}
		VALUE rzoom_x(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@zoom_x");
		}
		VALUE rzoom_xE(VALUE self, VALUE zoom_x)
		{
			CheckDisposed(self);
			Plane::get(self).setZoomX((float)NUM2DBL(zoom_x));
			return rb_iv_set(self, "@zoom_x", zoom_x);
		}
		VALUE rzoom_y(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@zoom_y");
		}
		VALUE rzoom_yE(VALUE self, VALUE zoom_y) {
			CheckDisposed(self);
			Plane::get(self).setZoomY((float)NUM2DBL(zoom_y));
			return rb_iv_set(self, "@zoom_y", zoom_y);
		}
		VALUE ropacity(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@opacity");
		}
		VALUE ropacityE(VALUE self, VALUE opacity) {
			CheckDisposed(self);
			Plane::get(self).setOpacity(NUM2INT(opacity));
			return rb_iv_set(self, "@opacity", opacity);
		}
		VALUE rblend_type(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@blend_type");
		}
		VALUE rblend_typeE(VALUE self, VALUE blend_type)
		{
			CheckDisposed(self);
			Plane::get(self).setBlendType( Blend::Type( NUM2INT(blend_type) ) );
			return rb_iv_set(self, "@blend_type", blend_type);
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
			Plane::get(self).setColor(color);
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
			Plane::get(self).setTone(tone);
			return rb_iv_set(self, "@tone", tone);
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Plane initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_class("Plane", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), -1 },
				{ ARGSS_FUNC(dispose), 0 }, { ARGSS_Q(disposed), 0 },
				ARGSS_GETTER_SETTER(viewport),
				ARGSS_GETTER_SETTER(bitmap),
				ARGSS_GETTER_SETTER(visible),
				ARGSS_GETTER_SETTER(z),
				ARGSS_GETTER_SETTER(ox),
				ARGSS_GETTER_SETTER(oy),
				ARGSS_GETTER_SETTER(zoom_x),
				ARGSS_GETTER_SETTER(zoom_y),
				ARGSS_GETTER_SETTER(opacity),
				ARGSS_GETTER_SETTER(blend_type),
				ARGSS_GETTER_SETTER(color),
				ARGSS_GETTER_SETTER(tone),
			};
			defineMethods(id, funcTable);
		}

		////////////////////////////////////////////////////////////
		/// Check if plane isn't disposed
		////////////////////////////////////////////////////////////
		void CheckDisposed(VALUE self)
		{
			if (Plane::IsDisposed(self)) {
				rb_raise(ARGSS::AError::getID(), "disposed plane <%i>", (int)self);
			}
		}
	} // namespace APlane
} // namespace ARGSS
