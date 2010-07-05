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
	void Draw(long z);
	void Draw(long z, Bitmap* dst_bitmap);

	void Flash(int duration);
	void Flash(Color color, int duration);
	void Update();
	VALUE getRect();
	void setRect(VALUE nrect);
	bool getVisible();
	void setVisible(bool nvisible);
	int getZ();
	void setZ(int nz);
	int getOx();
	void setOx(int nox);
	int getOy();
	void setOy(int noy);
	VALUE getColor();
	void setColor(VALUE ncolor);
	VALUE getTone();
	void setTone(VALUE ntone);

	void RegisterZObj(long z, VALUE id);
	void RegisterZObj(long z, VALUE id, bool multiz);
	void RemoveZObj(VALUE id);
	void UpdateZObj(VALUE id, long z);

	Rect const& getViewportRect() { return dstRect; }
private:
	std::list< ZObj > zlist;
	std::list< ZObj >::iterator it_zlist;

	VALUE id;
	VALUE rect;
	bool visible;
	int z;
	int ox;
	int oy;
	VALUE color;
	VALUE tone;

	Color flash_color;
	int flash_duration;
	int flash_frame;
	Color color_viewport;
	Tone tone_viewport;
	bool disposing;

	Bitmap* viewport;

	Rect dstRect;
}; // class Viewport

#endif
