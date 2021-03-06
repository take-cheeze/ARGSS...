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

#ifndef _TILEMAP_VX_HXX_
#define _TILEMAP_VX_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <boost/ptr_container/ptr_map.hpp>

#include <map>
#include <string>
#include <vector>

#include "bitmap.hxx"
#include "drawable.hxx"

////////////////////////////////////////////////////////////
/// Tilemap class
////////////////////////////////////////////////////////////
class Tilemap : public Drawable
{
public:
	Tilemap(VALUE iid);
	~Tilemap();

	static void Init();
	static bool IsDisposed(VALUE id);
	static void New(VALUE id);
	static Tilemap& get(VALUE id);
	static void Dispose(VALUE id);

	void RefreshBitmaps();
	void draw(long z);
	void draw(long z, Bitmap const& dst_bitmap);
	void RefreshData();

	void Update();
	VALUE getViewport() const { return viewport; }
	void setViewport(VALUE nviewport);
	VALUE getMapData() const { return map_data; }
	void setMapData(VALUE nmap_data);
	VALUE getFlashData() const { return flash_data; }
	void setFlashData(VALUE nflash_data) { flash_data = nflash_data; }
/*
	VALUE getTileset() const { return tileset; }
	void setTileset(VALUE ntileset) { tileset = ntileset; }
	VALUE getPriorities() const { return priorities; }
	void setPriorities(VALUE npriorities);
 */
	bool getVisible() const { return visible; }
	void setVisible(bool nvisible) { visible = nvisible; }
	int getOx() const { return ox_; }
	int getOy() const { return oy_; }
	void setOx(int nox) { ox_ = nox; }
	void setOy(int noy) { oy_ = noy; }

	VALUE getBitmaps() const { return bitmaps_; }
	void setBitmaps(VALUE nbitmaps) { bitmaps_ = nbitmaps; }
	VALUE getPassages() const { return passages_; }
	void setPassages(VALUE npassages) { passages_ = npassages; }
private:
	VALUE id;
	VALUE viewport;
	// VALUE tileset;
	// VALUE autotiles;
	VALUE map_data;
	VALUE flash_data;
	// VALUE priorities;
	bool visible;
	int ox_;
	int oy_;
	int autotile_frame;
	int autotile_time;
	VALUE bitmaps_;
	VALUE passages_;

	std::map< VALUE, std::map< int, boost::ptr_map<int, Bitmap> > > autotiles_cache;
	static int autotiles_id[6][8][4];

	struct TileData {
		int id;
		int priority;
	};
	std::vector< std::vector< std::vector<TileData> > > data_cache;
}; // class Tilemap

#endif // _TILEMAP_VX_HXX_
