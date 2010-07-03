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

#include "argss_bitmap.hxx"
#include "argss_color.hxx"
#include "argss_error.hxx"
#include "argss_font.hxx"

#include "bitmap.hxx"
#include "color.hxx"
#include "rect.hxx"


namespace ARGSS
{
	namespace ABitmap
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
		/// ARGSS Bitmap ruby functions
		////////////////////////////////////////////////////////////
		VALUE rinitialize(int argc, VALUE *argv, VALUE self) {
			switch(argc) {
				case 0: raise_argn(argc, 1);
					break;
				case 1: Bitmap::New(self, StringValuePtr(argv[0]));
					break;
				case 2: Bitmap::New(self, NUM2INT(argv[0]), NUM2INT(argv[1]));
					break;
				default: raise_argn(argc, 2);
					break;
			}
			rb_iv_set(self, "@font", ARGSS::AFont::New());
			ARGSS::ARuby::AddObject(self);
			return self;
		}
		VALUE rdispose(VALUE self) {
			if (!Bitmap::IsDisposed(self)) {
				Bitmap::Dispose(self);
				ARGSS::ARuby::RemoveObject(self);
				rb_gc_start();
			}
			return self;
		}
		VALUE rdisposedQ(VALUE self) {
			return INT2BOOL(Bitmap::IsDisposed(self));
		}
		VALUE rwidth(VALUE self) {
			CheckDisposed(self);
			return INT2NUM(Bitmap::Get(self).GetWidth());
		}
		VALUE rheight(VALUE self) {
			CheckDisposed(self);
			return INT2NUM(Bitmap::Get(self).GetHeight());
		}
		VALUE rrect(VALUE self) {
			CheckDisposed(self);
			return Bitmap::Get(self).GetRect().GetARGSS();
		}
		VALUE rblt(int argc, VALUE *argv, VALUE self) {
			CheckDisposed(self);
			if (argc < 4) raise_argn(argc, 4);
			else if (argc > 5) raise_argn(argc, 5);
			CheckDisposed(argv[2]);
			if (argc == 5) {
				Bitmap::Get(self).Blit(NUM2INT(argv[0]), NUM2INT(argv[1]), Bitmap::Get(argv[2]), Rect(argv[3]), NUM2INT(argv[4]));
			}
			else {
				Bitmap::Get(self).Blit(NUM2INT(argv[0]), NUM2INT(argv[1]), Bitmap::Get(argv[2]), Rect(argv[3]), 255);
			}
			return self;
		}
		VALUE rstretch_blt(int argc, VALUE *argv, VALUE self) {
			CheckDisposed(self);
			if (argc < 3) raise_argn(argc, 3);
			else if (argc > 4) raise_argn(argc, 4);
			CheckDisposed(argv[1]);
			if (argc == 4) {
				Bitmap::Get(self).StretchBlit(Rect(argv[0]), Bitmap::Get(argv[1]), Rect(argv[2]), NUM2INT(argv[3]));
			}
			else {
				Bitmap::Get(self).StretchBlit(Rect(argv[0]), Bitmap::Get(argv[1]), Rect(argv[2]), 255);
			}
			return self;
		}
		VALUE rfill_rect(int argc, VALUE *argv, VALUE self) {
			CheckDisposed(self);
			if (argc < 2) raise_argn(argc, 2);
			else if (argc == 2) {
				Bitmap::Get(self).FillRect(Rect(argv[0]), Color(argv[1]));
			}
			else if (argc == 5) {
				Bitmap::Get(self).FillRect(Rect(NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2INT(argv[2]), NUM2INT(argv[3])), Color(argv[4]));
			}
			else raise_argn(argc, 5);
			return self;
		}
		VALUE rclear(int argc, VALUE *argv, VALUE self) {
			CheckDisposed(self);
			if (argc > 1) raise_argn(argc, 1);
			else if (argc == 1) {
				Bitmap::Get(self).Clear(Color(argv[0]));
			}
			else {
				Bitmap::Get(self).Clear();
			}
			return self;
		}
		VALUE rget_pixel(VALUE self, VALUE x, VALUE y) {
			CheckDisposed(self);
			return Bitmap::Get(self).GetPixel(NUM2INT(x), NUM2INT(y)).GetARGSS();
		}
		VALUE rset_pixel(VALUE self, VALUE x, VALUE y, VALUE color) {
			CheckDisposed(self);
			Bitmap::Get(self).SetPixel(NUM2INT(x), NUM2INT(y), Color(color));
			return self;
		}
		VALUE rhue_change(VALUE self, VALUE hue) {
			CheckDisposed(self);
			Bitmap::Get(self).HueChange(NUM2DBL(hue));
			return self;
		}
		VALUE rsaturation_change(VALUE self, VALUE saturation) {
			CheckDisposed(self);
			Bitmap::Get(self).SatChange(NUM2DBL(saturation));
			return self;
		}
		VALUE rluminance_change(VALUE self, VALUE luminance) {
			CheckDisposed(self);
			Bitmap::Get(self).LumChange(NUM2DBL(luminance));
			return self;
		}
		VALUE rhsl_change(int argc, VALUE *argv, VALUE self) {
			CheckDisposed(self);
			if (argc < 3) raise_argn(argc, 3);
			else if (argc > 4) raise_argn(argc, 4);
			if (argc == 4) {
				Bitmap::Get(self).HSLChange(NUM2DBL(argv[0]), NUM2DBL(argv[1]), NUM2DBL(argv[2]), Rect(argv[3]));
			}
			else {
				Bitmap::Get(self).HSLChange(NUM2DBL(argv[0]), NUM2DBL(argv[1]), NUM2DBL(argv[2]));
			}
			return self;
		}
		VALUE rdraw_text(int argc, VALUE *argv, VALUE self) {
			CheckDisposed(self);
			int align = 0;
			if (argc < 2) raise_argn(argc, 2);
			else if (argc < 4) {
				if (argc == 3) {
					align = NUM2INT(argv[2]);
				}
				Bitmap::Get(self).TextDraw(Rect(argv[0]), StringValuePtr(argv[1]), align);
			}
			else if (argc == 4) raise_argn(argc, 3);
			else if (argc < 7) {
				if (argc == 6) {
					align = NUM2INT(argv[5]);
				}
				Bitmap::Get(self).TextDraw(Rect(NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2INT(argv[2]), NUM2INT(argv[3])), StringValuePtr(argv[4]), align);
			}
			else raise_argn(argc, 6);
			return self;
		}
		VALUE rtext_size(VALUE self, VALUE str) {
			CheckDisposed(self);
			return Bitmap::Get(self).GetTextSize(StringValuePtr(str)).GetARGSS();
		}
		VALUE rgradient_fill_rect(int argc, VALUE *argv, VALUE self) {
			CheckDisposed(self);
			if (argc < 3) raise_argn(argc, 3);
			else if (argc < 5) {
				bool vertical = false;
				if (argc == 4) {
					vertical = NUM2BOOL(argv[3]);
				}
			Bitmap::Get(self).GradientFillRect(Rect(argv[0]), Color(argv[1]), Color(argv[2]), vertical);
			}
			else if (argc < 6) raise_argn(argc, 6);
			else if (argc < 8) {
				bool vertical = false;
				if (argc == 4) {
					vertical = NUM2BOOL(argv[6]);
				}
				Bitmap::Get(self).GradientFillRect(Rect(NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2INT(argv[2]), NUM2INT(argv[3])), Color(argv[4]), Color(argv[5]), vertical);
			}
			else raise_argn(argc, 7);
			return self;
		}
		VALUE rclear_rect(int argc, VALUE *argv, VALUE self) {
			CheckDisposed(self);
			if (argc < 1) raise_argn(argc, 1);
			if (argc == 1) {
				Bitmap::Get(self).ClearRect(Rect(argv[0]));
			}
			else if (argc == 4) {
				Bitmap::Get(self).ClearRect(Rect(NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2INT(argv[2]), NUM2INT(argv[3])));
			}
			else raise_argn(argc, 4);
			return self;
		}
		VALUE rblur(VALUE self) {
			CheckDisposed(self);
			Bitmap::Get(self).Blur();
			return self;
		}
		VALUE rradial_blur(VALUE self, VALUE angle, VALUE division) {
			CheckDisposed(self);
			Bitmap::Get(self).RadialBlur(NUM2INT(angle), NUM2INT(division));
			return self;
		}
		VALUE rfont(VALUE self)
		{
			CheckDisposed(self);
			return rb_iv_get(self, "@font");
		}
		VALUE rfontE(VALUE self, VALUE font)
		{
			CheckDisposed(self);
			Check_Class( font, ARGSS::AFont::getID() );
			return rb_iv_set(self, "@font", font);;
		}
		VALUE rdup(VALUE self)
		{
			CheckDisposed(self);
			int width = Bitmap::Get(self).GetWidth();
			int height = Bitmap::Get(self).GetHeight();
			VALUE args[2] = {INT2NUM(width), INT2NUM(height)};
			VALUE bmp = rb_class_new_instance(2, args, id);
			Bitmap::Get(bmp).Blit(0, 0, Bitmap::Get(self), Rect(0, 0, width, height), 255);
			return bmp;
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Bitmap initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_class("Bitmap", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), -1 },
				{ ARGSS_FUNC(dispose), 0 }, { ARGSS_Q(disposed), 0 },
				{ ARGSS_FUNC(width), 0 }, { ARGSS_FUNC(height), 0 },
				{ ARGSS_FUNC(rect), 0 },
				{ ARGSS_FUNC(blt), -1 },
				{ ARGSS_FUNC(stretch_blt), -1 },
				{ ARGSS_FUNC(fill_rect), -1 },
				{ ARGSS_FUNC(clear), -1 },
				{ ARGSS_FUNC(get_pixel), 2 }, { ARGSS_FUNC(set_pixel), 3 },
				{ ARGSS_FUNC(hue_change), 1 },
				{ ARGSS_FUNC(saturation_change), 1 },
				{ ARGSS_FUNC(luminance_change), 1 },
				{ ARGSS_FUNC(hsl_change), -1 },
				{ ARGSS_FUNC(draw_text), -1 },
				{ ARGSS_FUNC(text_size), 1 },
				{ ARGSS_FUNC(gradient_fill_rect), -1 },
				{ ARGSS_FUNC(clear_rect), -1 },
				{ ARGSS_FUNC(blur), 0 },
				{ ARGSS_FUNC(radial_blur), 2 },
				ARGSS_GETTER_SETTER(font),
				{ "clone", RubyFunc(rdup), 0 },
				{ ARGSS_FUNC(dup), 0 },
			};
			defineMethods(id, funcTable);
		}

		////////////////////////////////////////////////////////////
		/// Check if bitmap isn't disposed
		////////////////////////////////////////////////////////////
		void CheckDisposed(VALUE self)
		{
			if (Bitmap::IsDisposed(self)) {
				rb_raise( ARGSS::AError::getID(), "disposed bitmap <%i>", int(self << 1) );
			}
		}
	} // namespace ABitmap
} // namespace ARGSS
