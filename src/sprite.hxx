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

#ifndef _SPRITE_HXX_
#define _SPRITE_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <memory>
#include <string>

#include "bitmap.hxx"
#include "color.hxx"
#include "drawable.hxx"
#include "rect.hxx"
#include "tone.hxx"

////////////////////////////////////////////////////////////
/// Sprite class
////////////////////////////////////////////////////////////
class Sprite : public Drawable
{
public:
	Sprite(VALUE iid);
	~Sprite();

	static bool IsDisposed(VALUE id);
	static void New(VALUE id);
	static Sprite& get(VALUE id);
	static void Dispose(VALUE id);

	void RefreshBitmaps();
	void draw(long z);
	void draw(long z, Bitmap const& dst_bitmap);

	void Flash(int duration);
	void Flash(Color const& color, int duration);
	void Update();

	VALUE getViewport() const { return viewport_; }
	void setViewport(VALUE nviewport);
	VALUE getBitmap() const { return bitmap_; }
	void setBitmap(VALUE nbitmap);
	VALUE getSrcRect() const { return srcRect_; }
	void setSrcRect(VALUE nsrcRect) { nsrcRect = srcRect_; }
	int getX() const { return x; }
	int getY() const { return y; }
	int getZ() const { return z; }
	void setX(int nx) { x = nx; }
	void setY(int ny) { y = ny; }
	void setZ(int nz);
	int getOx() const { return ox; }
	int getOy() const { return oy; }
	void setOx(int nox) { ox = nox; }
	void setOy(int noy) { oy = noy; }
	float getZoomX() const { return zoom_x; }
	float getZoomY() const { return zoom_y; }
	void setZoomX(float nzoom_x) { zoom_x = nzoom_x; }
	void setZoomY(float nzoom_y) { zoom_y = nzoom_y; }
	float getAngle() const { return angle; }
	void setAngle(float nangle) { angle = nangle; }
	void setFlipX(bool nflipx) { flipx = nflipx; }
	void setFlipY(bool nflipy) { flipy = nflipy; }
	bool getFlipX() const { return flipx; }
	bool getFlipY() const { return flipy; }
	int getBushDepth() const { return bush_depth; }
	void setBushDepth(int nbush_depth) { bush_depth = nbush_depth; }
	int getBushOpacity() const { return bush_opacity; }
	void setBushOpacity(int nbush_opacity) { bush_opacity = nbush_opacity; }
	int getOpacity() const { return opacity; }
	void setOpacity(int nopacity) { opacity = nopacity; }
	Blend::Type getBlendType() const { return blendType_; }
	void setBlendType(Blend::Type nBlendType) { blendType_ = nBlendType; }
	VALUE getColor() const { return color; }
	void setColor(VALUE ncolor) { color = ncolor; }
	VALUE getTone() const { return tone; }
	void setTone(VALUE ntone);

	bool getVisible() const { return visible; }
	void setVisible(bool nvisible) { visible = nvisible; }
/*
	bool getVisible() const { return NUM2BOOL( rb_iv_get(id, "@visible") ); }
	void setVisible(bool nvisible) { rb_iv_set( id, "@visible", BOOL2NUM(nvisible) ); }
 */
	int getWidth();
	int getHeight();
private:
	VALUE id;
	VALUE viewport_; // RGSS2
	VALUE bitmap_;
	VALUE srcRect_;
	bool visible;
	int x;
	int y;
	int z;
	int ox;
	int oy;
	float zoom_x;
	float zoom_y;
	float angle;
	bool flipx;
	bool flipy;
	int bush_depth;
	int bush_opacity; // RGSS2
	int opacity;
	Blend::Type blendType_;
	VALUE color;
	VALUE tone;

	int waveAmp_, waveLength_, waveSpeed_, wavePhase_; // RGSS2

	GLuint flash_texture;
	Color flash_color;
	int flash_duration;
	int flash_frame;
	std::auto_ptr< Bitmap > sprite_;
	Rect srcRect_sprite;
	Rect srcRect_last;
	bool needs_refresh;
	bool flash_needs_refresh;

	void Refresh();
	void RefreshFlash();
}; // class Sprite

#endif
