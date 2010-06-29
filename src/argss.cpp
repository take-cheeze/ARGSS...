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
#include "argss.h"
#include "argss_ruby.h"
#include "argss_output.h"
#include "argss_audio.h"
#include "argss_graphics.h"
#include "argss_input.h"
#include "argss_keys.h"
#include "argss_bitmap.h"
#include "argss_color.h"
#include "argss_font.h"
#include "argss_plane.h"
#include "argss_rect.h"
#include "argss_sprite.h"
#include "argss_table.h"
#include "argss_tilemap.h"
#include "argss_tone.h"
#include "argss_viewport.h"
#include "argss_window.h"
#include "argss_error.h"
#include "argss_rpg.h"
#include "argss_rpg_cache.h"
#include "argss_rpg_weather.h"
#include "argss_rpg_sprite.h"

#include <cstdlib>

////////////////////////////////////////////////////////////
/// ARGSS initialize
////////////////////////////////////////////////////////////
void ARGSS::Init() {
	VALUE lastGcSatate = rb_gc_disable();

	ARGSS::ARuby::Init();

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