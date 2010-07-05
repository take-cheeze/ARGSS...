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

#include "argss_color.hxx"
#include "argss_graphics.hxx"

#include "graphics.hxx"
#include "player.hxx"
#include "system.hxx"

namespace ARGSS
{
	namespace AGraphics
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
		/// ARGSS Graphics ruby functions
		////////////////////////////////////////////////////////////
		VALUE rupdate(VALUE self)
		{
			Graphics::Update();
			return Qnil;
		}
		VALUE rfreeze(VALUE self)
		{
			Graphics::Freeze();
			return Qnil;
		}
		VALUE rtransition(int argc, VALUE *argv, VALUE self)
		{
			if (argc > 3) raise_argn(argc, 3);
			int duration = 8;
			std::string filename;
			int vague = 40;
			if (argc > 0) {
				Check_Kind(argv[0], rb_cNumeric);
				duration = NUM2INT(argv[0]);
			}
			if (argc > 1) {
				Check_Type(argv[1], T_STRING);
				filename = StringValuePtr(argv[1]);
			}
			if (argc > 2) {
				Check_Kind(argv[2], rb_cNumeric);
				vague = NUM2INT(argv[2]);
			}
			Graphics::Transition(duration, filename, vague);
			return Qnil;
		}
		VALUE rframe_reset(VALUE self)
		{
			Graphics::FrameReset();
			return Qnil;
		}
		VALUE rframe_rate(VALUE self)
		{
			return INT2FIX(Graphics::getFrameRate());
		}
		VALUE rframe_rateE(VALUE self, VALUE framerate)
		{
			Check_Kind(framerate, rb_cNumeric);
			Graphics::setFrameRate(NUM2INT(framerate));
			return framerate;
		}
		VALUE rframe_count(VALUE self)
		{
			return INT2FIX(Graphics::getFrameCount());
		}
		VALUE rframe_countE(VALUE self, VALUE framecount)
		{
			Check_Kind(framecount, rb_cNumeric);
			Graphics::setFrameCount(NUM2INT(framecount));
			return framecount;
		}
		VALUE rfps(VALUE self)
		{
			return INT2FIX( Graphics::getFPS() );
		}
		VALUE rbackcolor(VALUE self)
		{
			return Graphics::getBackColor();
		}
		VALUE rbackcolorE(VALUE self, VALUE backcolor)
		{
			Check_Class( backcolor, ARGSS::AColor::getID() );
			Graphics::setBackColor(backcolor);
			return backcolor;
		}
		VALUE rwait(VALUE self, VALUE duration)
		{
			Check_Kind(duration, rb_cNumeric);
			Graphics::Wait(NUM2INT(duration));
			return Qnil;
		}
		VALUE rwidth(VALUE self)
		{
			return INT2NUM( Player::getMainWindow().getWidth() ); // System::Width);
		}
		VALUE rheight(VALUE self)
		{
			return INT2NUM( Player::getMainWindow().getHeight() ); // System::Height);
		}
		VALUE rresize_screen(VALUE self, VALUE width, VALUE height)
		{
			Check_Kind(width, rb_cNumeric);
			Check_Kind(height, rb_cNumeric);
			Graphics::ResizeScreen(NUM2INT(width), NUM2INT(height));
			return Qnil;
		}
		VALUE rsnap_to_bitmap(VALUE self)
		{
			return Graphics::SnapToBitmap();
		}
		VALUE rfadeout(VALUE self, VALUE duration)
		{
			Check_Kind(duration, rb_cNumeric);
			Graphics::FadeOut(NUM2INT(duration));
			return Qnil;
		}
		VALUE rfadein(VALUE self, VALUE duration)
		{
			Check_Kind(duration, rb_cNumeric);
			Graphics::FadeIn(NUM2INT(duration));
			return Qnil;
		}
		VALUE rbrightness(VALUE self)
		{
			return INT2FIX(Graphics::getBrightness());
		}
		VALUE rbrightnessE(VALUE self, VALUE brightness)
		{
			Check_Kind(brightness, rb_cNumeric);
			Graphics::setBrightness(NUM2INT(brightness));
			return brightness;
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Graphics initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_module("Graphics");
			rb_define_singleton_method(id, ARGSS_FUNC(update), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(freeze), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(transition), -1);
			rb_define_singleton_method(id, ARGSS_FUNC(frame_reset), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(frame_rate), 0);
			rb_define_singleton_method(id, ARGSS_E(frame_rate), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(frame_count), 0);
			rb_define_singleton_method(id, ARGSS_E(frame_count), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(fps), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(backcolor), 0);
			rb_define_singleton_method(id, ARGSS_E(backcolor), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(wait), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(width), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(height), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(resize_screen), 2);
			rb_define_singleton_method(id, ARGSS_FUNC(snap_to_bitmap), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(fadeout), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(fadein), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(brightness), 0);
			rb_define_singleton_method(id, ARGSS_E(brightness), 1);
		}
	} // namespace AGraphics
} // namespace ARGSS
