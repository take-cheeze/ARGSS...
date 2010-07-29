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

#ifndef _VIEWPORT_HXX_
#define _VIEWPORT_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <list>

#include "bitmap.hxx"
#include "color.hxx"
#include "rect.hxx"
#include "tone.hxx"
#include "drawable.hxx"
#include "zobj.hxx"

////////////////////////////////////////////////////////////
/// Viewport class
////////////////////////////////////////////////////////////
class Viewport : public Drawable
{
public:
	Viewport(VALUE iid);
	~Viewport();

	static bool IsDisposed(VALUE id);
	static void New(VALUE id);
	static Viewport& get(VALUE id);
	static void Dispose(VALUE id);

	void RefreshBitmaps();
	void draw(long z);
	void draw(long z, Bitmap const& dst_bitmap);

	void Flash(int duration);
	void Flash(Color const& color, int duration);
	void Update();
	VALUE getRect() const { return rect_; }
	void setRect(VALUE nrect) { rect_ = nrect; }
	bool getVisible() const { return visible_; }
	void setVisible(bool nvisible) { visible_ = nvisible; }
	int getZ() const { return z_; }
	void setZ(int nz);
	int getOx() const { return ox_; }
	int getOy() const { return oy_; }
	void setOx(int nox) { ox_ = nox; }
	void setOy(int noy) { oy_ = noy; }
	VALUE getColor() const { return color_; }
	void setColor(VALUE ncolor) { color_ = ncolor; }
	VALUE getTone() const { return tone_; }
	void setTone(VALUE ntone) { tone_ = ntone; }

	void RegisterZObj(long z, VALUE id);
	void RegisterZObj(long z, VALUE id, bool multiz);
	void RemoveZObj(VALUE id);
	void UpdateZObj(VALUE id, long z);

	Rect const& getViewportRect() { return dstRect_; }
private:
	std::list< ZObj > zlist_;

	VALUE id_;
	VALUE rect_;
	bool visible_;
	int z_;
	int ox_;
	int oy_;
	VALUE color_;
	VALUE tone_;

	Color flash_color_;
	int flash_duration_;
	int flash_frame_;
	Color color_viewport_;
	Tone tone_viewport_;
	bool disposing_;

	// Bitmap* viewport_;

	Rect dstRect_;
}; // class Viewport

#endif // _VIEWPORT_HXX_
