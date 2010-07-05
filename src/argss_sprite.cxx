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
#include "argss_sprite.hxx"
#include "argss_viewport.hxx"
#include "argss_bitmap.hxx"
#include "argss_color.hxx"
#include "argss_tone.hxx"
#include "argss_rect.hxx"
#include "argss_error.hxx"
#include "sprite.hxx"
#include "color.hxx"


namespace ARGSS
{
	namespace ASprite
	{
		namespace
		{
			////////////////////////////////////////////////////////////
			/// Global Variables
			////////////////////////////////////////////////////////////
			VALUE id;
		}
		VALUE& getID() { return id; }

		////////////////////////////////////////////////////////////
		/// ARGSS Sprite ruby functions
		////////////////////////////////////////////////////////////
		VALUE rinitialize(int argc, VALUE *argv, VALUE self) {
			if (argc == 1) {
				Check_Classes_N(argv[0], ARGSS::AViewport::getID());
				rb_iv_set(self, "@viewport", argv[0]);
			}
			else if (argc == 0) {
				rb_iv_set(self, "@viewport", Qnil);
			}
			else raise_argn(argc, 1);
			rb_iv_set(self, "@bitmap", Qnil);
			rb_iv_set(self, "@srcRect", ARGSS::ARect::New(0, 0, 0, 0));
			rb_iv_set(self, "@visible", Qtrue);
			rb_iv_set(self, "@x", INT2NUM(0));
			rb_iv_set(self, "@y", INT2NUM(0));
			rb_iv_set(self, "@z", INT2NUM(0));
			rb_iv_set(self, "@ox", INT2NUM(0));
			rb_iv_set(self, "@oy", INT2NUM(0));
			rb_iv_set(self, "@zoom_x", rb_float_new(1.0f));
			rb_iv_set(self, "@zoom_y", rb_float_new(1.0f));
			rb_iv_set(self, "@angle", rb_float_new(0.0f));
			rb_iv_set(self, "@flipx", Qfalse);
			rb_iv_set(self, "@flipy", Qfalse);
			rb_iv_set(self, "@bush_depth", INT2NUM(0));
			rb_iv_set(self, "@opacity", INT2NUM(255));
			rb_iv_set(self, "@blend_type", INT2NUM(0));
			rb_iv_set(self, "@color", ARGSS::AColor::New());
			rb_iv_set(self, "@tone", ARGSS::ATone::New());
			Sprite::New(self);
			ARGSS::ARuby::AddObject(self);
			return self;
		}
		VALUE rdispose(VALUE self) {
			if (!Sprite::IsDisposed(self)) {
				Check(self);
				Sprite::Dispose(self);
				ARGSS::ARuby::RemoveObject(self);
				rb_gc_start();
			}
			return self;
		}
		VALUE rdisposedQ(VALUE self) {
			return INT2BOOL(Sprite::IsDisposed(self));
		}
		VALUE rflash(VALUE self, VALUE color, VALUE duration) {
			Check(self);
			if (color == Qnil) {
				Sprite::get(self).Flash(NUM2INT(duration));
			}
			else {
				Sprite::get(self).Flash(Color(color), NUM2INT(duration));
			}
			return Qnil;
		}
		VALUE rupdate(VALUE self) {
			Check(self);
			Sprite::get(self).Update();
			return Qnil;
		}
		VALUE rwidth(VALUE self) {
			Check(self);
			return rb_iv_get(rb_iv_get(self, "@srcRect"), "@width");
		}
		VALUE rheight(VALUE self) {
			Check(self);
			return rb_iv_get(rb_iv_get(self, "@srcRect"), "@height");
		}
		VALUE rviewport(VALUE self) {
			Check(self);
			return rb_iv_get(self, "@viewport");
		}
		VALUE rviewportE(VALUE self, VALUE viewport) {
			Check(self);
			Check_Classes_N(viewport, ARGSS::AViewport::getID());
			if (viewport != Qnil) ARGSS::AViewport::CheckDisposed(viewport);
			Sprite::get(self).setViewport(viewport);
			return rb_iv_set(self, "@viewport", viewport);
		}
		VALUE rbitmap(VALUE self) {
			Check(self);
			return rb_iv_get(self, "@bitmap");
		}
		VALUE rbitmapE(VALUE self, VALUE bitmap) {
			Check(self);
			Check_Classes_N(bitmap, ARGSS::ABitmap::getID());
			if (bitmap != Qnil) {
				VALUE srcRect = rb_iv_get(self, "@srcRect");
				rb_iv_set(srcRect, "@x", INT2NUM(0));
				rb_iv_set(srcRect, "@y", INT2NUM(0));
				if (!Bitmap::IsDisposed(bitmap)) {
					rb_iv_set(srcRect, "@width", INT2NUM(Bitmap::get(bitmap).getWidth()));
					rb_iv_set(srcRect, "@height", INT2NUM(Bitmap::get(bitmap).getHeight()));
				}
				else {
					rb_iv_set(srcRect, "@width", INT2NUM(0));
					rb_iv_set(srcRect, "@height", INT2NUM(0));
				}
			}
			Sprite::get(self).setBitmap(bitmap);
			return rb_iv_set(self, "@bitmap", bitmap);
		}
		VALUE rsrcRect(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@srcRect");
		}
		VALUE rsrcRectE(VALUE self, VALUE srcRect)
		{
			Check(self);
			Check_Class(srcRect, ARGSS::ARect::getID());
			Sprite::get(self).setSrcRect(srcRect);
			return rb_iv_set(self, "@srcRect", srcRect);
		}
		VALUE rvisible(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@visible");
		}
		VALUE rvisibleE(VALUE self, VALUE visible)
		{
			Check(self);
			Sprite::get(self).setVisible(NUM2BOOL(visible));
			return rb_iv_set(self, "@visible", visible);
		}
		VALUE rx(VALUE self)
		{
			Check(self);
			return rb_to_int(rb_iv_get(self, "@x"));
		}
		VALUE rfx(VALUE self)
		{
			Check(self);
			return rb_Float(rb_iv_get(self, "@x"));
		}
		VALUE rxE(VALUE self, VALUE x)
		{
			Check(self);
			Sprite::get(self).setX(NUM2INT(x));
			return rb_iv_set(self, "@x", x);
		}
		VALUE ry(VALUE self)
		{
			Check(self);
			return rb_to_int(rb_iv_get(self, "@y"));
		}
		VALUE rfy(VALUE self)
		{
			Check(self);
			return rb_Float(rb_iv_get(self, "@y"));
		}
		VALUE ryE(VALUE self, VALUE y)
		{
			Check(self);
			Sprite::get(self).setY(NUM2INT(y));
			return rb_iv_set(self, "@y", y);
		}
		VALUE rz(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@z");
		}
		VALUE rzE(VALUE self, VALUE z)
		{
			Check(self);
			Sprite::get(self).setZ(NUM2INT(z));
			return rb_iv_set(self, "@z", rb_to_int(z));
		}
		VALUE rox(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@ox");
		}
		VALUE roxE(VALUE self, VALUE ox)
		{
			Check(self);
			Sprite::get(self).setOx(NUM2INT(ox));
			return rb_iv_set(self, "@ox", ox);
		}
		VALUE roy(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@oy");
		}
		VALUE royE(VALUE self, VALUE oy) {
			Check(self);
			Sprite::get(self).setOy(NUM2INT(oy));
			return rb_iv_set(self, "@oy", oy);
		}
		VALUE rzoom_x(VALUE self) {
			Check(self);
			return rb_iv_get(self, "@zoom_x");
		}
		VALUE rzoom_xE(VALUE self, VALUE zoom_x) {
			Check(self);
			Sprite::get(self).setZoomX((float)NUM2DBL(zoom_x));
			return rb_iv_set(self, "@zoom_x", rb_Float(zoom_x));
		}
		VALUE rzoom_y(VALUE self) {
			Check(self);
			return rb_iv_get(self, "@zoom_y");
		}
		VALUE rzoom_yE(VALUE self, VALUE zoom_y) {
			Check(self);
			Sprite::get(self).setZoomY((float)NUM2DBL(zoom_y));
			return rb_iv_set(self, "@zoom_y", rb_Float(zoom_y));
		}
		VALUE rangle(VALUE self) {
			Check(self);
			return rb_iv_get(self, "@angle");
		}
		VALUE rangleE(VALUE self, VALUE angle) {
			Check(self);
			Sprite::get(self).setAngle((float)NUM2DBL(angle));
			return rb_iv_set(self, "@angle", rb_Float(angle));
		}
		VALUE rmirror(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@flipx");
		}
		VALUE rmirrorE(VALUE self, VALUE mirror)
		{
			Check(self);
			Sprite::get(self).setFlipX(NUM2BOOL(mirror));
			return rb_iv_set(self, "@flipx", mirror);
		}
		VALUE rflipx(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@flipx");
		}
		VALUE rflipxE(VALUE self, VALUE flipx)
		{
			Check(self);
			Sprite::get(self).setFlipX(NUM2BOOL(flipx));
			return rb_iv_set(self, "@flipx", flipx);
		}
		VALUE rflipy(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@flipy");
		}
		VALUE rflipyE(VALUE self, VALUE flipy)
		{
			Check(self);
			Sprite::get(self).setFlipY(NUM2BOOL(flipy));
			return rb_iv_set(self, "@flipy", flipy);
		}
		VALUE rbush_depth(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@bush_depth");
		}
		VALUE rbush_depthE(VALUE self, VALUE bush_depth)
		{
			Check(self);
			Sprite::get(self).setBushDepth(NUM2INT(bush_depth));
			return rb_iv_set(self, "@bush_depth", bush_depth);
		}
		VALUE ropacity(VALUE self) {
			Check(self);
			return rb_iv_get(self, "@opacity");
		}
		VALUE ropacityE(VALUE self, VALUE opacity) {
			Check(self);
			Sprite::get(self).setOpacity(NUM2INT(opacity));
			return rb_iv_set(self, "@opacity", opacity);
		}
		VALUE rblend_type(VALUE self) {
			Check(self);
			return rb_iv_get(self, "@blend_type");
		}
		VALUE rblend_typeE(VALUE self, VALUE blend_type)
		{
			Check(self);
			int type = NUM2INT(blend_type);
			if (type < 0 || type > 3) type = Blend::NORMAL;
			Sprite::get(self).setBlendType( Blend::Type(type) );
			return rb_iv_set(self, "@blend_type", INT2NUM(type));
		}
		VALUE rcolor(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@color");
		}
		VALUE rcolorE(VALUE self, VALUE color)
		{
			Check(self);
			Check_Class(color, ARGSS::AColor::getID());
			Sprite::get(self).setColor(color);
			return rb_iv_set(self, "@color", color);
		}
		VALUE rtone(VALUE self)
		{
			Check(self);
			return rb_iv_get(self, "@tone");
		}
		VALUE rtoneE(VALUE self, VALUE tone)
		{
			Check(self);
			Check_Class(tone, ARGSS::ATone::getID());
			Sprite::get(self).setTone(tone);
			return rb_iv_set(self, "@tone", tone);
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Sprite initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_class("Sprite", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), -1 },
				{ ARGSS_FUNC(dispose), 0 }, { ARGSS_Q(disposed), 0 },
				{ ARGSS_FUNC(flash), 2 },
				{ ARGSS_FUNC(update), 0 },
				{ ARGSS_FUNC(width), 0 },
				{ ARGSS_FUNC(height), 0 },
				ARGSS_GETTER_SETTER(viewport),
				ARGSS_GETTER_SETTER(bitmap),
				ARGSS_GETTER_SETTER(srcRect),
				ARGSS_GETTER_SETTER(visible),
				ARGSS_GETTER_SETTER(x), { ARGSS_FUNC(fx), 0 },
				ARGSS_GETTER_SETTER(y), { ARGSS_FUNC(fy), 0 },
				ARGSS_GETTER_SETTER(z),
				ARGSS_GETTER_SETTER(ox),
				ARGSS_GETTER_SETTER(oy),
				ARGSS_GETTER_SETTER(zoom_x),
				ARGSS_GETTER_SETTER(zoom_y),
				ARGSS_GETTER_SETTER(angle),
				ARGSS_GETTER_SETTER(mirror),
				ARGSS_GETTER_SETTER(flipx),
				ARGSS_GETTER_SETTER(flipy),
				ARGSS_GETTER_SETTER(bush_depth),
				ARGSS_GETTER_SETTER(opacity),
				ARGSS_GETTER_SETTER(blend_type),
				ARGSS_GETTER_SETTER(color),
				ARGSS_GETTER_SETTER(tone),
			};
			defineMethods(id, funcTable);
		}

		////////////////////////////////////////////////////////////
		/// Check if sprite isn't disposed
		////////////////////////////////////////////////////////////
		void Check(VALUE self)
		{
			if (Sprite::IsDisposed(self)) {
				rb_raise(ARGSS::AError::getID(), "disposed sprite <%i>", (int)self);
			}
		}
	} // namespace ASprite
} // namespace ARGSS
