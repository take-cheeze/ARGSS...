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
	void draw(long z, Bitmap const& dst_bitmap);

	void Update();
	VALUE getViewport() const { return viewport_; }
	void setViewport(VALUE nviewport);
	VALUE getWindowskin() const { return windowskin_; }
	void setWindowskin(VALUE nwindowskin) { windowskin_ = nwindowskin; }
	VALUE getContents() const { return contents_; }
	void setContents(VALUE ncontents) { contents_ = ncontents; }
	bool getStretch() const { return stretch_; }
	void setStretch(bool nstretch) { stretch_ = nstretch; }
	VALUE getCursorRect() const { return cursor_rect_; }
	void setCursorRect(VALUE ncursor_rect) { cursor_rect_ = ncursor_rect; }
	bool getActive() const { return active_; }
	void setActive(bool nactive) { active_ = nactive; }
	bool getVisible() const { return visible_; }
	void setVisible(bool nvisible) { visible_ = nvisible; }
	bool getPause() const { return pause_; }
	void setPause(bool npause) { pause_ = npause; }
	int getX() const { return x_; }
	void setX(int nx) { x_ = nx; }
	int getY() const { return y_; }
	void setY(int ny) { y_ = ny; }
	int getWidth() const { return width_; }
	void setWidth(int nwidth) { width_ = nwidth; }
	int getHeight() const { return height_; }
	void setHeight(int nheight) { height_ = nheight; }
	int getZ() const { return z_; }
	void setZ(int nz);
	int getOx() const { return ox_; }
	void setOx(int nox) { ox_ = nox; }
	int getOy() const { return oy_; }
	void setOy(int noy) { oy_ = noy; }
	int getOpacity() const { return opacity_; }
	void setOpacity(int nopacity) { opacity_ = nopacity; }
	int getBackOpacity() const { return back_opacity_; }
	void setBackOpacity(int nback_opacity) { back_opacity_ = nback_opacity; }
	int getContentsOpacity() const { return contents_opacity_; }
	void setContentsOpacity(int ncontents_opacity) { contents_opacity_ = ncontents_opacity; }
private:
	VALUE id_;
	VALUE viewport_;
	VALUE windowskin_;
	VALUE contents_;
	bool stretch_;
	VALUE cursor_rect_;
	bool active_;
	bool visible_;
	bool pause_;
	int x_;
	int y_;
	int width_;
	int height_;
	int z_;
	int ox_;
	int oy_;
	int opacity_;
	int back_opacity_;
	int contents_opacity_;

	int cursor_frame_;
	int pause_frame_;
	int pause_id_;
}; // class Window

#endif // _WINDOW_XP_HXX_
