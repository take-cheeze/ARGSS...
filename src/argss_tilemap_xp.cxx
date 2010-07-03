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
#include "argss_error.hxx"
#include "argss_table.hxx"
#include "argss_tilemap_xp.hxx"
#include "argss_tilemapautotiles_xp.hxx"
#include "argss_viewport.hxx"

#include "tilemap_xp.hxx"


namespace ARGSS
{
	namespace ATilemap
	{
		namespace
		{
			////////////////////////////////////////////////////////////
			/// Global Variables
			////////////////////////////////////////////////////////////
			VALUE id;
		} // namespace

		////////////////////////////////////////////////////////////
		/// ARGSS Tilemap ruby functions
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
			rb_iv_set(self, "@tileset", Qnil);
			rb_iv_set(self, "@autotiles", ARGSS::ATilemapAutotiles::New());
			rb_iv_set(self, "@map_data", Qnil);
			rb_iv_set(self, "@flash_data", Qnil);
			rb_iv_set(self, "@priorities", Qnil);
			rb_iv_set(self, "@visible", Qtrue);
			rb_iv_set(self, "@ox", INT2NUM(0));
			rb_iv_set(self, "@oy", INT2NUM(0));
			Tilemap::New(self);
			ARGSS::ARuby::AddObject(self);
			return self;
		}
		VALUE rdispose(VALUE self) {
			if (!Tilemap::IsDisposed(self)) {
				CheckDisposed(self);
				Tilemap::Dispose(self);
				ARGSS::ARuby::RemoveObject(self);
				rb_gc_start();
			}
			return self;
		}
		VALUE rdisposedQ(VALUE self) {
			return INT2BOOL(Tilemap::IsDisposed(self));
		}
		VALUE rupdate(VALUE self) {
			CheckDisposed(self);
			Tilemap::Get(self).Update();
			return Qnil;
		}
		VALUE rviewport(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@viewport");
		}
		VALUE rviewportE(VALUE self, VALUE viewport) {
			CheckDisposed(self);
			Check_Classes_N(viewport, ARGSS::AViewport::getID());
			Tilemap::Get(self).SetViewport(viewport);
			return rb_iv_set(self, "@viewport", viewport);
		}
		VALUE rtileset(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@tileset");
		}
		VALUE rtilesetE(VALUE self, VALUE tileset) {
			CheckDisposed(self);
			Check_Classes_N(tileset, ARGSS::ABitmap::getID());
			Tilemap::Get(self).SetTileset(tileset);
			return rb_iv_set(self, "@tileset", tileset);
		}
		VALUE rautotiles(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@autotiles");
		}
		VALUE rmap_data(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@map_data");
		}
		VALUE rmap_dataE(VALUE self, VALUE map_data) {
			CheckDisposed(self);
			Check_Classes_N(map_data, ARGSS::ATable::getID());
			Tilemap::Get(self).SetMapData(map_data);
			return rb_iv_set(self, "@map_data", map_data);
		}
		VALUE rflash_data(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@flash_data");
		}
		VALUE rflash_dataE(VALUE self, VALUE flash_data) {
			CheckDisposed(self);
			Check_Classes_N(flash_data, ARGSS::ATable::getID());
			Tilemap::Get(self).SetFlashData(flash_data);
			return rb_iv_set(self, "@flash_data", flash_data);
		}
		VALUE rpriorities(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@priorities");
		}
		VALUE rprioritiesE(VALUE self, VALUE priorities) {
			CheckDisposed(self);
			Check_Classes_N(priorities, ARGSS::ATable::getID());
			Tilemap::Get(self).SetPriorities(priorities);
			return rb_iv_set(self, "@priorities", priorities);
		}
		VALUE rvisible(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@visible");
		}
		VALUE rvisibleE(VALUE self, VALUE visible) {
			CheckDisposed(self);
			Tilemap::Get(self).SetVisible(NUM2BOOL(visible));
			return rb_iv_set(self, "@visible", visible);
		}
		VALUE rox(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@ox");
		}
		VALUE roxE(VALUE self, VALUE ox) {
			CheckDisposed(self);
			Tilemap::Get(self).SetOx(NUM2INT(ox));
			return rb_iv_set(self, "@ox", ox);
		}
		VALUE roy(VALUE self) {
			CheckDisposed(self);
			return rb_iv_get(self, "@oy");
		}
		VALUE royE(VALUE self, VALUE oy) {
			CheckDisposed(self);
			Tilemap::Get(self).SetOy(NUM2INT(oy));
			return rb_iv_set(self, "@oy", oy);
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Tilemap initialize
		////////////////////////////////////////////////////////////
		void Init() {
			ARGSS::ATilemapAutotiles::Init();
			id = rb_define_class("Tilemap", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), -1 },
				{ ARGSS_FUNC(dispose), 0 }, { ARGSS_Q(disposed), 0 },
				{ ARGSS_FUNC(update), 0 },
				ARGSS_GETTER_SETTER(viewport),
				ARGSS_GETTER_SETTER(tileset),
				{ ARGSS_FUNC(autotiles), 0 },
				ARGSS_GETTER_SETTER(map_data),
				ARGSS_GETTER_SETTER(flash_data),
				ARGSS_GETTER_SETTER(priorities),
				ARGSS_GETTER_SETTER(visible),
				ARGSS_GETTER_SETTER(ox),
				ARGSS_GETTER_SETTER(oy),
			};
			defineMethods(id, funcTable);
		}

		////////////////////////////////////////////////////////////
		/// Check if tilemap isn't disposed
		////////////////////////////////////////////////////////////
		void CheckDisposed(VALUE self) {
			if (Tilemap::IsDisposed(self)) {
				rb_raise(ARGSS::AError::getID(), "disposed tilemap <%i>", (int)self);
			}
		}
	} // namespace ATilemap
} // namespace ARGSS
