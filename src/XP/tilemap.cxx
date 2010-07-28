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
#include <cmath>

#include <iostream>
#include <map>
#include <string>

#include <argss/ruby.hxx>
#include <argss/table.hxx>
#include <argss/tilemap.hxx>

#include "graphics.hxx"
#include "player.hxx"
#include "tilemap.hxx"
#include "viewport.hxx"

////////////////////////////////////////////////////////////
/// Static Variables
////////////////////////////////////////////////////////////
int Tilemap::autotiles_id[6][8][4] =
{ {
	{26, 27, 32, 33}, { 4, 27, 32, 33}, {26,  5, 32, 33}, { 4,  5, 32, 33},
	{26, 27, 32, 11}, { 4, 27, 32, 11}, {26,  5, 32, 11}, { 4,  5, 32, 11},
}, {
	{26, 27, 10, 33}, { 4, 27, 10, 33}, {26,  5, 10, 33}, { 4,  5, 10, 33},
	{26, 27, 10, 11}, { 4, 27, 10, 11}, {26,  5, 10, 11}, { 4,  5, 10, 11},
}, {
	{24, 25, 30, 31}, {24,  5, 30, 31}, {24, 25, 30, 11}, {24,  5, 30, 11},
	{14, 15, 20, 21}, {14, 15, 20, 11}, {14, 15, 10, 21}, {14, 15, 10, 11},
}, {
	{28, 29, 34, 35}, {28, 29, 10, 35}, { 4, 29, 34, 35}, { 4, 29, 10, 35},
	{38, 39, 44, 45}, { 4, 39, 44, 45}, {38,  5, 44, 45}, { 4,  5, 44, 45},
}, {
	{24, 29, 30, 35}, {14, 15, 44, 45}, {12, 13, 18, 19}, {12, 13, 18, 11},
	{16, 17, 22, 23}, {16, 17, 10, 23}, {40, 41, 46, 47}, { 4, 41, 46, 47},
}, {
	{36, 37, 42, 43}, {36,  5, 42, 43}, {12, 17, 18, 23}, {12, 13, 42, 43},
	{36, 41, 42, 47}, {16, 17, 46, 47}, {12, 17, 42, 47}, { 0,  1,  6,  7},
}, };

namespace
{
	int const TILE_SIZE = 32;
}

////////////////////////////////////////////////////////////
/// Class Init
////////////////////////////////////////////////////////////
void Tilemap::Init()
{
/*
	int temp[192] = { 26, 27, 32, 33,  4, 27, 32, 33, 26,  5, 32, 33,  4,  5, 32, 33,
					  26, 27, 32, 11,  4, 27, 32, 11, 26,  5, 32, 11,  4,  5, 32, 11,
					  26, 27, 10, 33,  4, 27, 10, 33, 26,  5, 10, 33,  4,  5, 10, 33,
					  26, 27, 10, 11,  4, 27, 10, 11, 26,  5, 10, 11,  4,  5, 10, 11,
					  24, 25, 30, 31, 24,  5, 30, 31, 24, 25, 30, 11, 24,  5, 30, 11,
					  14, 15, 20, 21, 14, 15, 20, 11, 14, 15, 10, 21, 14, 15, 10, 11,
					  28, 29, 34, 35, 28, 29, 10, 35,  4, 29, 34, 35,  4, 29, 10, 35,
					  38, 39, 44, 45,  4, 39, 44, 45, 38,  5, 44, 45,  4,  5, 44, 45,
					  24, 29, 30, 35, 14, 15, 44, 45, 12, 13, 18, 19, 12, 13, 18, 11,
					  16, 17, 22, 23, 16, 17, 10, 23, 40, 41, 46, 47,  4, 41, 46, 47,
					  36, 37, 42, 43, 36,  5, 42, 43, 12, 17, 18, 23, 12, 13, 42, 43,
					  36, 41, 42, 47, 16, 17, 46, 47, 12, 17, 42, 47,  0,  1,  6,  7};
	memcpy(autotiles_id, temp, 192 * sizeof(int)); 
 */
}

////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////
Tilemap::Tilemap(VALUE iid)
: id(iid)
, viewport(rb_iv_get(id, "@viewport"))
, tileset(Qnil)
, autotiles(rb_iv_get(id, "@autotiles"))
, map_data(Qnil)
, flash_data(Qnil)
, priorities(Qnil)
, visible(true)
, ox_(0)
, oy_(0)
, autotile_frame(0)
, autotile_time(0)
{
}

////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Tilemap::~Tilemap()
{
}

////////////////////////////////////////////////////////////
/// Class Is Tilemap Disposed?
////////////////////////////////////////////////////////////
bool Tilemap::IsDisposed(VALUE id)
{
	return Graphics::countDrawable(id) == 0;
}

////////////////////////////////////////////////////////////
/// Class New Tilemap
////////////////////////////////////////////////////////////
void Tilemap::New(VALUE id)
{
	Graphics::insertDrawable( id, boost::shared_ptr< Drawable >( new Tilemap(id) ) );
}

////////////////////////////////////////////////////////////
/// Class get Tilemap
////////////////////////////////////////////////////////////
Tilemap& Tilemap::get(VALUE id)
{
	return dynamic_cast< Tilemap& >( Graphics::getDrawable(id) );
}


////////////////////////////////////////////////////////////
/// Class Dispose Tilemap
////////////////////////////////////////////////////////////
void Tilemap::Dispose(VALUE id)
{
	if ( ! NIL_P( Tilemap::get(id).viewport ) ) Viewport::get(Tilemap::get(id).viewport).RemoveZObj(id);
	else Graphics::RemoveZObj(id);

	Graphics::eraseDrawable(id);
}

////////////////////////////////////////////////////////////
/// Update
////////////////////////////////////////////////////////////
void Tilemap::Update()
{
	autotile_time += 1;
	if (autotile_time == 8) {
		autotile_time = 0;
		autotile_frame += 1;
	}
}

////////////////////////////////////////////////////////////
/// Refresh Bitmaps
////////////////////////////////////////////////////////////
void Tilemap::RefreshBitmaps()
{
	std::map< VALUE, std::map< int, std::map< int, boost::shared_ptr< Bitmap > > > >::iterator it1;
	std::map< int, std::map< int, boost::shared_ptr< Bitmap > > >::iterator it2;
	std::map< int, boost::shared_ptr< Bitmap > >::iterator it3;
	for (it1 = autotiles_cache.begin(); it1 != autotiles_cache.end(); it1++) {
		for (it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			for (it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
				it3->second->Changed();
			}
		}
	}
}

////////////////////////////////////////////////////////////
/// Draw
////////////////////////////////////////////////////////////
void Tilemap::draw(long z_level)
{
	if (!visible) return;
	if ( NIL_P(tileset) || NIL_P(map_data) || NIL_P(priorities) ) return;

	if (z_level == 0) {
		RefreshData();
		Bitmap::get(tileset).Refresh();
	}

	int width = NUM2INT(rb_iv_get(map_data, "@xsize"));
	int height = NUM2INT(rb_iv_get(map_data, "@ysize"));
	int layers = NUM2INT(rb_iv_get(map_data, "@zsize"));

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if ( ! NIL_P(viewport) ) {
		Rect const& rect = Viewport::get(viewport).getViewportRect();

		glEnable(GL_SCISSOR_TEST);
		glScissor(rect.x, Player::getHeight() - (rect.y + rect.height), rect.width, rect.height);

		glTranslatef((float)rect.x, (float)rect.y, 0.0f);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	int tiles_x = (int)std::ceil(Player::getWidth () / double(TILE_SIZE));
	int tiles_y = (int)std::ceil(Player::getHeight() / double(TILE_SIZE));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	for (int z = 0; z < layers; z++) {
		for (int y = 0; y <= tiles_y; y++) {
			for (int x = 0; x <= tiles_x; x++) {
				int map_x = ox_ / TILE_SIZE + x;
				int map_y = oy_ / TILE_SIZE + y;

				if (map_x >= width || map_y >= height) continue;

				TileData const& tile = data_cache[map_x][map_y][z];

				int tile_z;
				if (tile.priority == 0) tile_z = 0;
				else {
					tile_z = tile.priority * TILE_SIZE + y * TILE_SIZE + z * TILE_SIZE;
					if (map_y == 0 && tile.priority == 1) tile_z += TILE_SIZE;
				}

				if( (tile_z == z_level) && (tile.id != 0) ) {
					float dst_x = (float)(x * TILE_SIZE - ox_ % TILE_SIZE);
					float dst_y = (float)(y * TILE_SIZE - oy_ % TILE_SIZE);
					GLfloat texCoords[4][2];
					GLfloat vertexes[4][2];

					vertexes[0][0] = dst_x; vertexes[0][1] = dst_y;
					vertexes[1][0] = dst_x + TILE_SIZE; vertexes[1][1] = dst_y;
					vertexes[2][0] = dst_x + TILE_SIZE; vertexes[2][1] = dst_y + TILE_SIZE;
					vertexes[3][0] = dst_x; vertexes[3][1] = dst_y + TILE_SIZE;

					if(tile.id < 384) {
						VALUE bitmap_id = rb_ary_entry(rb_iv_get(autotiles, "@autotiles"), tile.id / 48 - 1);
						if (Bitmap::IsDisposed(bitmap_id)) continue;
						Bitmap& autotile_bitmap = Bitmap::get(bitmap_id);
						int tile_id = tile.id % 48;
						int frame = autotile_frame % (autotile_bitmap.getWidth() / 96);

						if (autotiles_cache.count(bitmap_id) == 0 ||
								autotiles_cache[bitmap_id].count(tile_id) == 0 ||
								autotiles_cache[bitmap_id][tile_id].count(frame) == 0) {
							autotiles_cache[bitmap_id][tile_id][frame] = boost::shared_ptr< Bitmap >( new Bitmap(TILE_SIZE, TILE_SIZE) );
							int* tiles = autotiles_id[tile_id >> 3][tile_id & 7];
							for (int i = 0; i < 4; i++) {
								Rect rect(((tiles[i] % 6) << 4) + frame * 96, (tiles[i] / 6) << 4, 16, 16);
								autotiles_cache[bitmap_id][tile_id][frame]->Blit((i % 2) << 4, (i / 2) << 4, autotile_bitmap, rect, 255);
							}
							glPushMatrix();

							autotiles_cache[bitmap_id][tile_id][frame]->Refresh();

							glMatrixMode(GL_MODELVIEW);
							glPopMatrix();
						}
						autotiles_cache[bitmap_id][tile_id][frame]->BindBitmap();

						texCoords[0][0] = 0.f; texCoords[0][1] = 0.f;
						texCoords[1][0] = 1.f; texCoords[1][1] = 0.f;
						texCoords[2][0] = 1.f; texCoords[2][1] = 1.f;
						texCoords[3][0] = 0.f; texCoords[3][1] = 1.f;
					} else {
						float src_x = (float)((tile.id - 384) % 8 * TILE_SIZE);
						float src_y = (float)((tile.id - 384) / 8 * TILE_SIZE);

						Bitmap::get(tileset).BindBitmap();

						float bmpw = (float)Bitmap::get(tileset).getWidth();
						float bmph = (float)Bitmap::get(tileset).getHeight();

						texCoords[0][0] = src_x / bmpw; texCoords[0][1] = src_y / bmph;
						texCoords[1][0] = (src_x + TILE_SIZE) / bmpw; texCoords[1][1] = src_y / bmph;
						texCoords[2][0] = (src_x + TILE_SIZE) / bmpw; texCoords[2][1] = (src_y + TILE_SIZE) / bmph;
						texCoords[3][0] = src_x / bmpw; texCoords[3][1] = (src_y + TILE_SIZE) / bmph;
					}
					glVertexPointer(2, GL_FLOAT, 0, vertexes);
					glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
					glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
				}
			}
		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_SCISSOR_TEST);
}
void Tilemap::draw(long z, Bitmap const& dst_bitmap)
{
}

////////////////////////////////////////////////////////////
/// Refresh Data
////////////////////////////////////////////////////////////
void Tilemap::RefreshData()
{
	bool update = false;
	if (rb_iv_get(map_data, "@modified") == Qtrue) {
		rb_iv_set(map_data, "@modified", Qfalse);
		update = true;
	}
	if (rb_iv_get(priorities, "@modified") == Qtrue) {
		rb_iv_set(priorities, "@modified", Qfalse);
		update = true;
	}

	if (!update) return;

	int width = NUM2INT(rb_iv_get(map_data, "@xsize"));
	int height = NUM2INT(rb_iv_get(map_data, "@ysize"));
	int layers = NUM2INT(rb_iv_get(map_data, "@zsize"));

	VALUE map_data_array = rb_iv_get(map_data, "@data");
	VALUE priorities_array = rb_iv_get(priorities, "@data");

	data_cache.resize(width);
	for (int x = 0; x < width; x++) {
		data_cache[x].resize(height);
		for (int y = 0; y < height; y++) {
			data_cache[x][y].resize(layers);
			for (int z = 0; z < layers; z++) {
				TileData tile;
				tile.id = NUM2INT(rb_ary_entry(map_data_array, x + y * width + z * width * height));
				tile.priority = NUM2INT(rb_ary_entry(priorities_array, tile.id));
				data_cache[x][y][z] = tile;
			}
		}
	}
}

////////////////////////////////////////////////////////////
/// Properties
////////////////////////////////////////////////////////////
void Tilemap::setViewport(VALUE nviewport)
{
	if (viewport != nviewport) {
		if ( ! NIL_P(viewport) ) Viewport::get(viewport).RemoveZObj(id);
		else Graphics::RemoveZObj(id);

		int height = NUM2INT(rb_iv_get(map_data, "@ysize"));
		if ( ! NIL_P(nviewport) ) {
			for (int i = 0; i < height + 5; i++) {
				Viewport::get(nviewport).RegisterZObj(i * TILE_SIZE, id, true);
			}
		} else {
			for (int i = 0; i < height + 5; i++) {
				Graphics::RegisterZObj(i * TILE_SIZE, id, true);
			}
		}
	}
	viewport = nviewport;
}
void Tilemap::setMapData(VALUE nmap_data)
{
	if (map_data != nmap_data) {
		if ( ! NIL_P(viewport) ) Viewport::get(viewport).RemoveZObj(id);
		else Graphics::RemoveZObj(id);

		if ( ! NIL_P(nmap_data) ) {
			rb_iv_set(nmap_data, "@modified", Qtrue);
			// int height = // NIL_P(rb_iv_get(nmap_data, "@ysize"))? 0 : // sometimes the ysize is NIL
				// NUM2INT(rb_iv_get(nmap_data, "@ysize"));
			int height = ARGSS::ATable::getTable(nmap_data).ysize();
			// std::cout << height << std::endl;
			if ( ! NIL_P(viewport) ) {
				for (int i = 0; i < height + 8; i++) {
					Viewport::get(viewport).RegisterZObj(i * TILE_SIZE, id, true);
				}
			} else {
				for (int i = 0; i < height + 8; i++) {
					Graphics::RegisterZObj(i * TILE_SIZE, id, true);
				}
			}
		}
	}
	map_data = nmap_data;
}
void Tilemap::setPriorities(VALUE npriorities)
{
	if (priorities != npriorities) rb_iv_set(npriorities, "@modified", Qtrue);
	priorities = npriorities;
}
