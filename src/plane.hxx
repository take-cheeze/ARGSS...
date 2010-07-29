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

#include <argss/ruby.hxx>

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
	static Plane& get(VALUE id);
	static void Dispose(VALUE id);

	void RefreshBitmaps();
	void draw(long z);
	void draw(long z, Bitmap const& dst_bitmap);

	VALUE getViewport() { return viewport_; }
	void setViewport(VALUE nviewport);
	VALUE getBitmap() { return bitmap_; }
	void setBitmap(VALUE nbitmap);
	bool getVisible() { return visible_; }
	void setVisible(bool nvisible) { visible_ = nvisible; }
	int getZ() { return z; }
	void setZ(int nz);
	int getOx() { return ox; }
	int getOy() { return oy; }
	void setOx(int nox) { ox = nox; }
	void setOy(int noy) { oy = noy; }
	float getZoomX() { return zoom_x; }
	float getZoomY() { return zoom_y; }
	void setZoomX(float nzoom_x);
	void setZoomY(float nzoom_y);
	int getOpacity() { return opacity; }
	void setOpacity(int nopacity);
	Blend::Type getBlendType() { return blendType_; }
	void setBlendType(Blend::Type nBlendType) { blendType_ = nBlendType; }
	VALUE getColor(){ return color; }
	void setColor(VALUE ncolor) { color = ncolor; }
	VALUE getTone() { return tone; }
	void setTone(VALUE ntone);
private:
	void Refresh();
	void ApplyTone();
	void ApplyZoom();
	bool needs_refresh;
	Tone tone_last;

	VALUE id;
	VALUE viewport_;
	VALUE bitmap_;
	bool visible_;
	int z;
	int ox;
	int oy;
	float zoom_x;
	float zoom_y;
	int opacity;
	Blend::Type blendType_;
	VALUE color;
	VALUE tone;
}; // class Plane

#endif // _PLANE_HXX_
