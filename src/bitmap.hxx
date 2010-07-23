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

#ifndef _BITMAP_HXX_
#define _BITMAP_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <boost/smart_ptr.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <GL/gl.h>

#include <argss/ruby.hxx>

#include "color.hxx"
#include "defines.hxx"
#include "rect.hxx"
#include "tone.hxx"

////////////////////////////////////////////////////////////
/// Bitmap class
////////////////////////////////////////////////////////////
class Bitmap
{
public:
	Bitmap();
	Bitmap(int iwidth, int iheight);
	Bitmap(VALUE iid, std::string const& filename);
	Bitmap(VALUE iid, int iwidth, int iheight);
	Bitmap(Bitmap& source, Rect const& srcRect);
	~Bitmap();

	static bool IsDisposed(VALUE id);
	static void New(VALUE id, std::string const& filename);
	static void New(VALUE id, int width, int height);
	static Bitmap& get(VALUE id);
	static void Dispose(VALUE id);
	static void RefreshBitmaps();
	static void DisposeBitmaps();

	int getWidth () const { return  width_; }
	int getHeight() const { return height_; }
	void Copy(int x, int y, Bitmap const& source, Rect srcRect);
	void Blit(int x, int y, Bitmap const& source, Rect srcRect, int opacity);
	void StretchBlit(Rect const& dstRect, Bitmap const& srcBitmap, Rect srcRect, int opacity);
	void FillRect(Rect rect, Color const& color);
	void Clear();
	void Clear(Color const& color);
	Color getPixel(int x, int y);
	void setPixel(int x, int y, Color const& color);
	void HueChange(double hue);
	void SatChange(double saturation);
	void LumChange(double luminance);
	void HSLChange(double h, double s, double l);
	void HSLChange(double h, double s, double l, Rect rect);
	void TextDraw(Rect const& rect, std::string const& text, int align);
	Rect getTextSize(std::string const& text);
	void GradientFillRect(Rect const& rect, Color const& color1, Color const& color2, bool vertical);
	void ClearRect(Rect rect);
	void Blur();
	void RadialBlur(int angle, int division);

	void ToneChange(Tone tone);
	void OpacityChange(int opacity, int bush_depth = 0);
	void Flip(bool flipx, bool flipy);
	void Zoom(double zoom_x, double zoom_y);
	std::auto_ptr< Bitmap > Resample(int scalew, int scaleh, Rect const& srcRect) const;
	void Rotate(float angle);

	Rect getRect() { return Rect(0, 0, getWidth(), getHeight()); }

	void Changed();
	void Refresh();
	void BindBitmap();
	Uint32* getPixels() { return &pixels_[0]; }
protected:
	VALUE id;

	GLuint glBitmap_;
	long width_;
	long height_;

	std::vector< Uint32 > pixels_;
}; // class Bitmap

#endif
