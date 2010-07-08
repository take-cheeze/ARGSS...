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
#include <argss/argss.hxx>
#include <argss/ruby.hxx>
#include <argss/output.hxx>
#include <argss/audio.hxx>
#include <argss/graphics.hxx>
#include <argss/input.hxx>
#include <argss/keys.hxx>
#include <argss/bitmap.hxx>
#include <argss/color.hxx>
#include <argss/font.hxx>
#include <argss/plane.hxx>
#include <argss/rect.hxx>
#include <argss/sprite.hxx>
#include <argss/table.hxx>
#include <argss/tilemap.hxx>
#include <argss/tone.hxx>
#include <argss/viewport.hxx>
#include <argss/window.hxx>
#include <argss/error.hxx>
#include <argss/rpg.hxx>
#include <argss/rpg_cache.hxx>
#include <argss/rpg_weather.hxx>
#include <argss/rpg_sprite.hxx>

#include <cstdlib>

////////////////////////////////////////////////////////////
/// ARGSS initialize
////////////////////////////////////////////////////////////
void ARGSS::Init() {
	ARGSS::ARuby::Init();

	/*
	 * initialize ruby before disabling GC
	 */
	VALUE lastGcSatate = rb_gc_disable();

	ARGSS::AOutput::Init();
	ARGSS::AAudio::Init();
	ARGSS::AGraphics::Init();
	ARGSS::AInput::Init();
	ARGSS::AKeys::Init();

	ARGSS::ABitmap::Init();
	ARGSS::AColor::Init();
	ARGSS::AFont::Init();
	ARGSS::APlane::Init();
	ARGSS::ARect::Init();
	ARGSS::ASprite::Init();
	ARGSS::ATable::Init();
	ARGSS::ATilemap::Init();
	ARGSS::ATone::Init();
	ARGSS::AViewport::Init();
	ARGSS::AWindow::Init();
	ARGSS::AError::Init();

	ARGSS::ARPG::Init();
	ARGSS::ARPG::ACache::Init();
	ARGSS::ARPG::AWeather::Init();
	ARGSS::ARPG::ASprite::Init();

	if( lastGcSatate ) {
		rb_gc();
		rb_gc_enable();
	}

 	ARGSS::ARuby::Run();
}

////////////////////////////////////////////////////////////
/// ARGSS Exit
////////////////////////////////////////////////////////////
void ARGSS::Exit() {
	rb_exit(EXIT_FAILURE);
}
