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
	static Sprite& Get(VALUE id);
	static void Dispose(VALUE id);

	void RefreshBitmaps();
	void Draw(long z);
	void Draw(long z, Bitmap* dst_bitmap);

	void Flash(int duration);
	void Flash(Color color, int duration);
	void Update();
	VALUE GetViewport();
	void SetViewport(VALUE nviewport);
	VALUE GetBitmap();
	void SetBitmap(VALUE nbitmap);
	VALUE GetSrcRect();
	void SetSrcRect(VALUE nsrc_rect);
	bool GetVisible();
	void SetVisible(bool nvisible);
	int GetX();
	void SetX(int nx);
	int GetY();
	void SetY(int ny);
	int GetZ();
	void SetZ(int nz);
	int GetOx();
	void SetOx(int nox);
	int GetOy();
	void SetOy(int noy);
	float GetZoomX();
	void SetZoomX(float nzoom_x);
	float GetZoomY();
	void SetZoomY(float nzoom_y);
	float GetAngle();
	void SetAngle(float nangle);
	bool GetFlipX();
	void SetFlipX(bool nflipx);
	bool GetFlipY();
	void SetFlipY(bool nflipy);
	int GetBushDepth();
	void SetBushDepth(int nbush_depth);
	int GetOpacity();
	void SetOpacity(int nopacity);
	int GetBlendType();
	void SetBlendType(int nblend_type);
	VALUE GetColor();
	void SetColor(VALUE ncolor);
	VALUE GetTone();
	void SetTone(VALUE ntone);

private:
	VALUE id;
	VALUE viewport;
	VALUE bitmap;
	VALUE src_rect;
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
	int opacity;
	int blend_type;
	VALUE color;
	VALUE tone;

	GLuint flash_texture;
	Color flash_color;
	int flash_duration;
	int flash_frame;
	Bitmap* sprite;
	Rect src_rect_sprite;
	Rect src_rect_last;
	bool needs_refresh;
	bool flash_needs_refresh;

	void Refresh();
	void RefreshFlash();
	int GetWidth();
	int GetHeight();
}; // class Sprite

#endif
