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

#ifndef _ARGSS_GRAPHICS_HXX_
#define _ARGSS_GRAPHICS_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <argss/ruby.hxx>

////////////////////////////////////////////////////////////
/// ARGSS Graphics namespace
////////////////////////////////////////////////////////////
namespace ARGSS
{
	namespace AGraphics
	{
		VALUE& getID();

		void Init();

		VALUE rupdate(VALUE self);
		VALUE rfreeze(VALUE self);
		VALUE rtransition(int argc, VALUE *argv, VALUE self);
		VALUE rframe_reset(VALUE self);
		VALUE rframe_rate(VALUE self);
		VALUE rframe_rateE(VALUE self, VALUE framerate);
		VALUE rframe_count(VALUE self);
		VALUE rframe_countE(VALUE self, VALUE framecount);
		VALUE rfps(VALUE self);
		VALUE rbackcolor(VALUE self);
		VALUE rbackcolorE(VALUE self, VALUE backcolor);
		VALUE rwait(VALUE self, VALUE duration);
		VALUE rwidth (VALUE self); // RGSS2
		VALUE rheight(VALUE self); // RGSS2
		VALUE rresize_screen(VALUE self, VALUE width, VALUE height); // RGSS2
		VALUE rsnap_to_bitmap(VALUE self); // RGSS2
		VALUE rfadeout(VALUE self, VALUE duration); // RGSS2
		VALUE rfadein (VALUE self, VALUE duration); // RGSS2
		VALUE rbrightness(VALUE self); // RGSS2
		VALUE rbrightnessE(VALUE self, VALUE brightness); // RGSS2
	} // namespace AGraphics
} // namespace ARGSS

#endif // _ARGSS_GRAPHICS_HXX_
