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

#ifndef _PLANE_HXX_
#define _PLANE_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <string>

#include "argss_ruby.hxx"

#include "bitmap.hxx"
#include "drawable.hxx"
#include "tone.hxx"

////////////////////////////////////////////////////////////
/// Plane class
////////////////////////////////////////////////////////////
class Plane : public Drawable
{
public:
	Plane(VALUE iid);
	~Plane();

	static bool IsDisposed(VALUE id);
	static void New(VALUE id);
	static Plane& Get(VALUE id);
	static void Dispose(VALUE id);

	void RefreshBitmaps();
	void Draw(long z);
	void Draw(long z, Bitmap* dst_bitmap);

	VALUE GetViewport();
	void SetViewport(VALUE nviewport);
	VALUE GetBitmap();
	void SetBitmap(VALUE nbitmap);
	bool GetVisible();
	void SetVisible(bool nvisible);
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
	int GetOpacity();
	void SetOpacity(int nopacity);
	int GetBlendType();
	void SetBlendType(int nblend_type);
	VALUE GetColor();
	void SetColor(VALUE ncolor);
	VALUE GetTone();
	void SetTone(VALUE ntone);

private:
	void Refresh();
	void ApplyTone();
	void ApplyZoom();
	bool needs_refresh;
	Tone tone_last;

	VALUE id;
	VALUE viewport;
	VALUE bitmap;
	bool visible;
	int z;
	int ox;
	int oy;
	float zoom_x;
	float zoom_y;
	int opacity;
	int blend_type;
	VALUE color;
	VALUE tone;
}; // class Plane

#endif
