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
	void Draw(long z);
	void Draw(long z, Bitmap* dst_bitmap);

	void Update();
	VALUE getViewport();
	void setViewport(VALUE nviewport);
	VALUE getWindowskin();
	void setWindowskin(VALUE nwindowskin);
	VALUE getContents();
	void setContents(VALUE ncontents);
	bool getStretch();
	void setStretch(bool nstretch);
	VALUE getCursorRect();
	void setCursorRect(VALUE ncursor_rect);
	bool getActive();
	void setActive(bool nactive);
	bool getVisible();
	void setVisible(bool nvisible);
	bool getPause();
	void setPause(bool npause);
	int getX();
	void setX(int nx);
	int getY();
	void setY(int ny);
	int getWidth();
	void setWidth(int nwidth);
	int getHeight();
	void setHeight(int nheight);
	int getZ();
	void setZ(int nz);
	int getOx();
	void setOx(int nox);
	int getOy();
	void setOy(int noy);
	int getOpacity();
	void setOpacity(int nopacity);
	int getBackOpacity();
	void setBackOpacity(int nback_opacity);
	int getContentsOpacity();
	void setContentsOpacity(int ncontents_opacity);

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
