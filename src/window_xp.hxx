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

#ifndef _WINDOW_XP_HXX_
#define _WINDOW_XP_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <string>

#include "bitmap.hxx"
#include "drawable.hxx"

////////////////////////////////////////////////////////////
/// Window class
////////////////////////////////////////////////////////////
class Window : public Drawable
{
public:
	Window(VALUE iid);
	~Window();

	static bool IsDisposed(VALUE id);
	static void New(VALUE id);
	static Window& get(VALUE id);
	static void Dispose(VALUE id);

	void RefreshBitmaps();
	void draw(long z);
	void draw(long z, Bitmap* dst_bitmap);

	void Update();
	VALUE getViewport() const { return viewport; }
	void setViewport(VALUE nviewport);
	VALUE getWindowskin() const { return windowskin; }
	void setWindowskin(VALUE nwindowskin) { windowskin = nwindowskin; }
	VALUE getContents() const { return contents; }
	void setContents(VALUE ncontents) { contents = ncontents; }
	bool getStretch() const { return stretch; }
	void setStretch(bool nstretch) { stretch = nstretch; }
	VALUE getCursorRect() const { return cursor_rect; }
	void setCursorRect(VALUE ncursor_rect) { cursor_rect = ncursor_rect; }
	bool getActive() const { return active; }
	void setActive(bool nactive) { active = nactive; }
	bool getVisible() const { return visible; }
	void setVisible(bool nvisible) { visible = nvisible; }
	bool getPause() const { return pause; }
	void setPause(bool npause) { pause = npause; }
	int getX() const { return x; }
	void setX(int nx) { x = nx; }
	int getY() const { return y; }
	void setY(int ny) { y = ny; }
	int getWidth() const { return width; }
	void setWidth(int nwidth) { width = nwidth; }
	int getHeight() const { return height; }
	void setHeight(int nheight) { height = nheight; }
	int getZ() const { return z; }
	void setZ(int nz);
	int getOx() const { return ox; }
	void setOx(int nox) { ox = nox; }
	int getOy() const { return oy; }
	void setOy(int noy) { oy = noy; }
	int getOpacity() const { return opacity; }
	void setOpacity(int nopacity) { opacity = nopacity; }
	int getBackOpacity() const { return back_opacity; }
	void setBackOpacity(int nback_opacity) { back_opacity = nback_opacity; }
	int getContentsOpacity() const { return contents_opacity; }
	void setContentsOpacity(int ncontents_opacity) { contents_opacity = ncontents_opacity; }
private:
	VALUE id;
	VALUE viewport;
	VALUE windowskin;
	VALUE contents;
	bool stretch;
	VALUE cursor_rect;
	bool active;
	bool visible;
	bool pause;
	int x;
	int y;
	int width;
	int height;
	int z;
	int ox;
	int oy;
	int opacity;
	int back_opacity;
	int contents_opacity;

	int cursor_frame;
	int pause_frame;
	int pause_id;
}; // class Window

#endif
