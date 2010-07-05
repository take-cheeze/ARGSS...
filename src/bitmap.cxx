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

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstring>

#include <SOIL.h>

#include <argss/error.hxx>

#include "text.hxx"
#include "bitmap.hxx"
#include "defines.hxx"
#include "hslrgb.hxx"
#include "graphics.hxx"
#include "system.hxx"
#include "filefinder.hxx"

////////////////////////////////////////////////////////////
/// Static Variables
////////////////////////////////////////////////////////////
std::map< VALUE, boost::shared_ptr< Bitmap > > Bitmap::bitmaps_;

////////////////////////////////////////////////////////////
/// Constructors
////////////////////////////////////////////////////////////
Bitmap::Bitmap(int iwidth, int iheight)
{
	id = 0;
	pixels_.resize(iwidth * iheight, 0);
	width_ = (long)iwidth;
	height_ = (long)iheight;
	glBitmap_ = 0;
}
Bitmap::Bitmap(VALUE iid, std::string const& filename)
{
	std::string path = FileFinder::FindImage(filename);
	if ( path.empty() ) {
		return;
		VALUE enoent = rb_const_get(rb_mErrno, rb_intern("ENOENT"));
		rb_raise(enoent, "No such file or directory - %s", filename.c_str());
	}

	int rwidth, rheight, channels;
	unsigned char* load_pixels = SOIL_load_image(path.c_str(), &rwidth, &rheight, &channels, SOIL_LOAD_RGBA);

	if (!load_pixels) {
		rb_raise(ARGSS::AError::getID(), "couldn't load %s image.\n%s\n", filename.c_str(), SOIL_last_result());
	}

	pixels_.resize(rwidth * rheight);
	std::memcpy(&pixels_[0], load_pixels, rwidth * rheight * 4);

	SOIL_free_image_data(load_pixels);

	id = iid;
	width_ = (long)rwidth;
	height_ = (long)rheight;
	glBitmap_ = 0;
}
Bitmap::Bitmap(VALUE iid, int iwidth, int iheight)
{
	if (iwidth <= 0 && iheight <= 0) {
		rb_raise(ARGSS::AError::getID(), "cant't create %dx%d image.\nWidth and height_ must be bigger than 0.\n", iwidth, iheight);
	}
	id = iid;
	pixels_.resize(iwidth * iheight, 0);
	width_ = (long)iwidth;
	height_ = (long)iheight;
	glBitmap_ = 0;
}
Bitmap::Bitmap(Bitmap& source, Rect const& srcRect)
{
	if (srcRect.width <= 0 && srcRect.height <= 0) {
		rb_raise(ARGSS::AError::getID(), "cant't create %dx%d image.\nWidth and height_ must be bigger than 0.\n", srcRect.width, srcRect.height);
	}
	id = 0;
	width_ = (long)srcRect.width;
	height_ = (long)srcRect.height;
	pixels_.resize(width_ * height_, 0);
	glBitmap_ = 0;
	Copy(0, 0, source, srcRect);
}

////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Bitmap::~Bitmap()
{
	if (glBitmap_ > 0)  glDeleteTextures(1, &glBitmap_);
}

////////////////////////////////////////////////////////////
/// Class Is Bitmap Disposed?
////////////////////////////////////////////////////////////
bool Bitmap::IsDisposed(VALUE id)
{
	return bitmaps_.count(id) == 0;
}

////////////////////////////////////////////////////////////
/// Class New Bitmap
////////////////////////////////////////////////////////////
void Bitmap::New(VALUE id, std::string const& filename)
{
	bitmaps_[id] = boost::shared_ptr< Bitmap >( new Bitmap(id, filename) );
}
void Bitmap::New(VALUE id, int width_, int height_)
{
	bitmaps_[id] = boost::shared_ptr< Bitmap >( new Bitmap(id, width_, height_) );
}

////////////////////////////////////////////////////////////
/// Class get Bitmap
////////////////////////////////////////////////////////////
Bitmap& Bitmap::get(VALUE id)
{
	assert( bitmaps_.find(id) != bitmaps_.end() );
	return *bitmaps_.find(id)->second;
}

////////////////////////////////////////////////////////////
/// Class Dispose Bitmap
////////////////////////////////////////////////////////////
void Bitmap::Dispose(VALUE id)
{
	assert( bitmaps_.find(id) != bitmaps_.end() );
	bitmaps_.erase( bitmaps_.find(id) );
}

////////////////////////////////////////////////////////////
/// Class Refresh Bitmaps
////////////////////////////////////////////////////////////
void Bitmap::RefreshBitmaps()
{
	std::map< VALUE, boost::shared_ptr< Bitmap > >::iterator it_bitmaps;
	for (it_bitmaps = bitmaps_.begin(); it_bitmaps != bitmaps_.end(); it_bitmaps++) {
		it_bitmaps->second->Changed();
	}
}

////////////////////////////////////////////////////////////
/// Class Dispose Bitmaps
////////////////////////////////////////////////////////////
void Bitmap::DisposeBitmaps()
{
	bitmaps_.clear();
}

////////////////////////////////////////////////////////////
/// Changed
////////////////////////////////////////////////////////////
void Bitmap::Changed() {
	if (glBitmap_ > 0) {
		glEnable(GL_TEXTURE_2D);

		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();

		glBindTexture(GL_TEXTURE_2D, glBitmap_);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, &pixels_[0]);
		//glDeleteTextures(1, &glBitmap_);
		//glBitmap_ = 0;
	}
}

////////////////////////////////////////////////////////////
/// Refresh
////////////////////////////////////////////////////////////
void Bitmap::Refresh()
{
	if (glBitmap_ > 0) return;

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glGenTextures(1, &glBitmap_);
	glBindTexture(GL_TEXTURE_2D, glBitmap_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels_[0]);
}

////////////////////////////////////////////////////////////
/// Bind Bitmap texture
////////////////////////////////////////////////////////////
void Bitmap::BindBitmap()
{
	glBindTexture(GL_TEXTURE_2D, glBitmap_);
}

////////////////////////////////////////////////////////////
/// Copy
////////////////////////////////////////////////////////////
void Bitmap::Copy(int x, int y, Bitmap const& srcBitmap, Rect srcRect)
{
	if (srcBitmap.getWidth() == 0 || srcBitmap.getHeight() == 0 || width_ == 0 || height_ == 0) return;
	if (x >= width_ || y >= height_) return;

	if (x < 0) {
		srcRect.x -= x;
		x = 0;
	}
	if (y < 0) {
		srcRect.y -= y;
		y = 0;
	}
	
	srcRect.Adjust(srcBitmap.getWidth(), srcBitmap.getHeight());
	if (srcRect.IsOutOfBounds(srcBitmap.getWidth(), srcBitmap.getHeight())) return;

	int src_width = srcRect.width;
	int src_height = srcRect.height;
	if (x + src_width  > width_) src_width = width_ - x;
	if (y + src_height > height_) src_height = height_ - y;    
	if (src_width <= 0 || src_height <= 0)  return;

	int src_pitch = src_width * 4;
	int src_row = srcBitmap.getWidth();
	Uint32 const* src_pixels = ((Uint32*)(&srcBitmap.pixels_[0])) + srcRect.x + srcRect.y * srcBitmap.getWidth();
	Uint32* dst_pixels = ((Uint32*)(&pixels_[0])) + x + y * width_;

	for (int i = 0; i < src_height; ++i) {
		std::memcpy(dst_pixels, src_pixels, src_pitch);
		src_pixels += src_row;
		dst_pixels += width_;
	}

	Changed();
}

////////////////////////////////////////////////////////////
/// Blit
////////////////////////////////////////////////////////////
void Bitmap::Blit(int x, int y, Bitmap const& srcBitmap, Rect srcRect, int opacity)
{
	if (srcBitmap.getWidth() == 0 || srcBitmap.getHeight() == 0 || width_ == 0 || height_ == 0) return;
	if (x >= width_ || y >= height_) return;

	if (x < 0) {
		srcRect.x -= x;
		x = 0;
	}
	if (y < 0) {
		srcRect.y -= y;
		y = 0;
	}

	srcRect.Adjust(srcBitmap.getWidth(), srcBitmap.getHeight());
	if (srcRect.IsOutOfBounds(srcBitmap.getWidth(), srcBitmap.getHeight())) return;

	int src_width = srcRect.width;
	int src_height = srcRect.height;
	if (x + src_width  > width_) src_width = width_ - x;
	if (y + src_height > height_) src_height = height_ - y;    
	if (src_width <= 0 || src_height <= 0)  return;

	int src_row = srcBitmap.getWidth() * 4;
	int dst_row = width_ * 4;
	Uint8 const* src_pixels = ((Uint8*)(&srcBitmap.pixels_[0])) + (srcRect.x + srcRect.y * srcBitmap.getWidth()) * 4;
	Uint8* dst_pixels = ((Uint8*)(&pixels_[0])) + (x + y * width_) * 4;

	if (opacity > 255) opacity = 255;
	if (opacity > 0) {
		for (int i = 0; i < src_height; ++i) {
			for (int j = 0; j < src_width; ++j) {
				Uint8 const* src = src_pixels + j * 4;
				Uint8      * dst = dst_pixels + j * 4;

				Uint8 srca = src[3] * opacity / 255;
				dst[0] = (dst[0] * (255 - srca) + src[0] * srca) / 255;
				dst[1] = (dst[1] * (255 - srca) + src[1] * srca) / 255;
				dst[2] = (dst[2] * (255 - srca) + src[2] * srca) / 255;
				dst[3] = dst[3] * (255 - srca) / 255 + srca;
			}
			src_pixels += src_row;
			dst_pixels += dst_row;
		}
	}

	Changed();
}

////////////////////////////////////////////////////////////
/// Stretch blit
////////////////////////////////////////////////////////////
void Bitmap::StretchBlit(Rect const& dstRect, Bitmap const& srcBitmap, Rect srcRect, int opacity)
{
	if (srcRect.width == dstRect.width && srcRect.height == dstRect.height) {
		Blit(dstRect.x, dstRect.y, srcBitmap, srcRect, opacity);
	} else {
		srcRect.Adjust(srcBitmap.getWidth(), srcBitmap.getHeight());
		if (srcRect.IsOutOfBounds(srcBitmap.getWidth(), srcBitmap.getHeight())) return;

		std::auto_ptr< Bitmap > resampled = srcBitmap.Resample(dstRect.width, dstRect.height, srcRect);
		Rect rect(0, 0, dstRect.width, dstRect.height);
		Blit(dstRect.x, dstRect.y, *resampled, rect, opacity);
	}

	Changed();
}

////////////////////////////////////////////////////////////
/// Fill rect
////////////////////////////////////////////////////////////
void Bitmap::FillRect(Rect rect, Color const& color)
{
	rect.Adjust(getWidth(), getHeight());
	if (rect.IsOutOfBounds(getWidth(), getHeight())) return;

	long dst = rect.x + rect.y * width_;
	Uint32 col = color.getUint32();
	for (int i = 0; i < rect.height; i++) {
		fill_n(pixels_.begin() + dst, rect.width, col);
		dst += width_;
	}

	Changed();
}

////////////////////////////////////////////////////////////
/// Clear
////////////////////////////////////////////////////////////
void Bitmap::Clear()
{
	std::memset(&pixels_[0], 0, width_ * height_ * 4);

	Changed();
}
void Bitmap::Clear(Color const& color)
{
	fill_n(pixels_.begin(), width_ * height_, color.getUint32());

	Changed();
}
void Bitmap::ClearRect(Rect rect)
{
	if( rect == getRect() ) Clear();
	else {
		rect.Adjust(width_, height_);
		for(int i = 0; i < rect.height; i++) {
			std::memset( &pixels_[rect.x + width_ * i], 0x0, rect.width * sizeof(Uint32) );
		}
	}
}

////////////////////////////////////////////////////////////
/// get pixel
////////////////////////////////////////////////////////////
Color Bitmap::getPixel(int x, int y)
{
	if (x < 0 || y < 0) return Color(0, 0, 0, 0);
	if (x >= getWidth() || y > getHeight()) return Color(0, 0, 0, 0);
	return Color::NewUint32(pixels_[(y * width_) + x]);
}

////////////////////////////////////////////////////////////
/// set pixel
////////////////////////////////////////////////////////////
void Bitmap::setPixel(int x, int y, Color const& color)
{
	if (x < 0 || y < 0) return;
	if (x >= width_ || y > height_) return;
	pixels_[(y * width_) + x] = color.getUint32();

	Changed();
}

////////////////////////////////////////////////////////////
/// Hue change
////////////////////////////////////////////////////////////
void Bitmap::HueChange(double hue)
{
	for(int i = 0; i < height_; i++) {
		for(int j = 0; j < width_; j++) {
			Color color = Color::NewUint32(pixels_[(i * width_) + j]);
			pixels_[(i * width_) + j] = RGBAdjustHSL(color, hue, 0, 1).getUint32();
		}
	}

	Changed();
}

////////////////////////////////////////////////////////////
/// Saturation change
////////////////////////////////////////////////////////////
void Bitmap::SatChange(double saturation)
{
	for(int i = 0; i < height_; i++) {
		for(int j = 0; j < width_; j++) {
			Color color = Color::NewUint32(pixels_[(i * width_) + j]);
			pixels_[(i * width_) + j] = RGBAdjustHSL(color, 0, saturation, 1).getUint32();
		}
	}

	Changed();
}

////////////////////////////////////////////////////////////
/// Lustd::minance change
////////////////////////////////////////////////////////////
void Bitmap::LumChange(double luminance)
{
	for(int i = 0; i < height_; i++) {
		for(int j = 0; j < width_; j++) {
			Color color = Color::NewUint32(pixels_[(i * width_) + j]);
			pixels_[(i * width_) + j] = RGBAdjustHSL(color, 0, 0, luminance).getUint32();
		}
	}

	Changed();
}

////////////////////////////////////////////////////////////
/// HSL change
////////////////////////////////////////////////////////////
void Bitmap::HSLChange(double h, double s, double l)
{
	for(int i = 0; i < height_; i++) {
		for(int j = 0; j < width_; j++) {
			Color color = Color::NewUint32(pixels_[(i * width_) + j]);
			pixels_[(i * width_) + j] = RGBAdjustHSL(color, h, s, l).getUint32();
		}
	}

	Changed();
}
void Bitmap::HSLChange(double h, double s, double l, Rect rect)
{
	rect.Adjust(getWidth(), getHeight());
	if (rect.IsOutOfBounds(getWidth(), getHeight())) return;

	for(int i = rect.y; i < rect.y + rect.height; i++) {
		for(int j = rect.x; j < rect.x + rect.width; j++) {
			Color color = Color::NewUint32(pixels_[(i * width_) + j]);
			pixels_[(i * width_) + j] = RGBAdjustHSL(color, h, s, l).getUint32();
		}
	}

	Changed();
}

////////////////////////////////////////////////////////////
/// Draw text
////////////////////////////////////////////////////////////
void Bitmap::TextDraw(Rect const& rect, std::string const& text, int align)
{
	if (text.length() == 0) return;
	if (rect.IsOutOfBounds(getWidth(), getHeight())) return;

	VALUE font_id = rb_iv_get(id, "@font");
	VALUE name_id = rb_iv_get(font_id, "@name");
	Color color = Color(rb_iv_get(font_id, "@color"));
	int size = NUM2INT(rb_iv_get(font_id, "@size"));
	bool bold = NUM2BOOL(rb_iv_get(font_id, "@bold"));
	bool italic = NUM2BOOL(rb_iv_get(font_id, "@italic"));

	std::auto_ptr< Bitmap > text_bmp = Text::Draw(text, StringValuePtr(name_id), color, size, bold, italic, false);

	if (text_bmp->getWidth() > rect.width) {
		int stretch = (int)(text_bmp->getWidth() * 0.4);
		if (rect.width > stretch) stretch = rect.width;
		Rect resample_rect(0, 0, text_bmp->getWidth(), text_bmp->getHeight());
		text_bmp = text_bmp->Resample(stretch, text_bmp->getHeight(), resample_rect);;
	}
	Rect srcRect(0, 0, rect.width, rect.height);
	int y = rect.y;
	if (rect.height > text_bmp->getHeight()) y += ((rect.height - text_bmp->getHeight()) / 2);
	int x = rect.x;
	if (rect.width > text_bmp->getWidth()) {
		if (align == 1) x += (rect.width - text_bmp->getWidth()) / 2;
		else if (align == 2) x += rect.width - text_bmp->getWidth();
	}
	Blit(x, y, *text_bmp, srcRect, (int)color.alpha);

	Changed();
}

////////////////////////////////////////////////////////////
/// get text size
////////////////////////////////////////////////////////////
Rect Bitmap::getTextSize(std::string const& text) {
	VALUE font_id = rb_iv_get(id, "@font");
	VALUE name_id = rb_iv_get(font_id, "@name");
	int size = NUM2INT(rb_iv_get(font_id, "@size"));
	return Text::RectSize(text, StringValuePtr(name_id), size);
}

////////////////////////////////////////////////////////////
/// Gradient fill rect
////////////////////////////////////////////////////////////
void Bitmap::GradientFillRect(Rect const& rect, Color const& color1, Color const& color2, bool vertical)
{
	// TODO
}

////////////////////////////////////////////////////////////
/// Blur
////////////////////////////////////////////////////////////
void Bitmap::Blur()
{
	// TODO
}

////////////////////////////////////////////////////////////
/// Radial Blur
////////////////////////////////////////////////////////////
void Bitmap::RadialBlur(int angle, int division)
{
	// TODO
}

////////////////////////////////////////////////////////////
/// Tone change
////////////////////////////////////////////////////////////
void Bitmap::ToneChange(Tone tone)
{
	if (tone.red == 0 && tone.green == 0 && tone.blue == 0 && tone.gray == 0) return;

	Uint8 *dst_pixels = (Uint8*)&pixels_[0];
	
	if (tone.gray == 0) {
		for (int i = 0; i < getHeight(); i++) {
			for (int j = 0; j < getWidth(); j++) {
				Uint8 *pixel = dst_pixels;
				pixel[0] = (Uint8)std::max(std::min( int(pixel[0] + tone.red  ), 255), 0);
				pixel[1] = (Uint8)std::max(std::min( int(pixel[1] + tone.green), 255), 0);
				pixel[2] = (Uint8)std::max(std::min( int(pixel[2] + tone.blue ), 255), 0);
				dst_pixels += 4;
			}
		}
	}
	else {
		double factor = (255 - tone.gray) / 255.0;
		double gray;
		for (int i = 0; i < getHeight(); i++) {
			for (int j = 0; j < getWidth(); j++) {
				Uint8 *pixel = dst_pixels;
				
				gray = pixel[0] * 0.299 + pixel[1] * 0.587 + pixel[2] * 0.114;
				pixel[0] = (Uint8)std::max( std::min( int( ( pixel[0] - gray) * factor + gray + tone.red   + 0.5 ), 255 ), 0 );
				pixel[1] = (Uint8)std::max( std::min( int( ( pixel[1] - gray) * factor + gray + tone.green + 0.5 ), 255 ), 0 );
				pixel[2] = (Uint8)std::max( std::min( int( ( pixel[2] - gray) * factor + gray + tone.blue  + 0.5 ), 255 ), 0 );
				dst_pixels += 4;
			}
		}
	}

	Changed();
}

////////////////////////////////////////////////////////////
/// Opacity change
////////////////////////////////////////////////////////////
void Bitmap::OpacityChange(int opacity, int bush_depth)
{
	/*if (opacity >= 255 && bush_depth <= 0) return;

	SDL_LockSurface(bitmap);

	Uint8 *pixels_ = (Uint8 *)bitmap->pixels_;

	int start_bush = std::max(getHeight() - bush_depth, 0);

	const int abyte = MaskgetByte(bitmap->format->Amask);

	if (opacity < 255) {
		for (int i = 0; i < start_bush; i++) {
			for (int j = 0; j < getWidth(); j++) {
				Uint8 *pixel = pixels_;
				pixel[abyte] = pixel[abyte] * opacity / 255;
				pixels_ += 4;
			}
		}
		for (int i = start_bush; i< getHeight(); i++) {
			for (int j = 0; j < getWidth(); j++) {
				Uint8 *pixel = pixels_;
				pixel[abyte] = (pixel[abyte] / 2) * opacity / 255;
				pixels_ += 4;
			}
		}
	}
	else {
		pixels_ += start_bush * getWidth() * 4;
		for (int i = start_bush; i < getHeight(); i++) {
			for (int j = 0; j < getWidth(); j++) {
				Uint8 *pixel = pixels_;
				pixel[abyte] = pixel[abyte] / 2;
				pixels_ += 4;
			}
		}
	}
	SDL_UnlockSurface(bitmap);

	Changed();*/
}

////////////////////////////////////////////////////////////
/// Flip
////////////////////////////////////////////////////////////
void Bitmap::Flip(bool flipx, bool flipy)
{
	/*if (!(flipx || flipy)) return;

	SDL_LockSurface(bitmap);

	Bitmap* temp_bmp = new Bitmap(getWidth(), getHeight());
	SDL_Surface* temp = temp_bmp->bitmap;

	SDL_LockSurface(temp);

	Uint32* srcpixels = (Uint32*)bitmap->pixels_;
	Uint32* dstpixels = (Uint32*)temp->pixels_;

	if (flipx && flipy) {
		long srcpixel = 0;
		long dstpixel = getWidth() + (getHeight() - 1) * getWidth() - 1;
		for(int i = 0; i < getHeight(); i++) {
			for(int j = 0; j < getWidth(); j++) {
				dstpixels[dstpixel] = srcpixels[srcpixel];
				
				srcpixel += 1;
				dstpixel -= 1;
			}
		}
	}
	else if (flipx) {
		long srcpixel = 0;
		long dstpixel = getWidth() - 1;
		for(int i = 0; i < getHeight(); i++) {
			for(int j = 0; j < getWidth(); j++) {
				dstpixels[dstpixel] = srcpixels[srcpixel];

				srcpixel += 1;
				dstpixel -= 1;
			}
			dstpixel += getWidth() * 2;
		}
	}
	else if (flipy) {
		dstpixels += (getHeight() - 1) * getWidth();
		for(int i = 0; i < getHeight(); i++) {
			std::memcpy(dstpixels, srcpixels,  getWidth() * 4);
			srcpixels += getWidth();
			dstpixels -= getWidth();
		}
	}

	SDL_UnlockSurface(bitmap);
	SDL_UnlockSurface(temp);

	SDL_FreeSurface(bitmap);

	bitmap = temp;

	temp_bmp->bitmap = NULL;
	delete temp_bmp;

	Changed();*/
}

////////////////////////////////////////////////////////////
/// Zoom
////////////////////////////////////////////////////////////
void Bitmap::Zoom(double zoom_x, double zoom_y)
{
	/*if (zoom_x == 1.0 && zoom_y == 1.0) return;

	SDL_LockSurface(bitmap);

	int scalew = (int)(getWidth() * zoom_x);
	int scaleh = (int)(getHeight() * zoom_y);

	Bitmap* nbitmap = new Bitmap(scalew, scaleh);
	SDL_Surface* surface = nbitmap->bitmap;

	SDL_LockSurface(surface);

	Uint8* srcpixels = (Uint8*)bitmap->pixels_;
	Uint8* dstpixels = (Uint8*)surface->pixels_;

	int row = getWidth() * 4;

	for(int yy = 0; yy < surface->h; yy++) {
		int nearest_matchy = (int)(yy / zoom_y) * row;
		for(int xx = 0; xx < surface->w; xx++) {
			int nearest_match = nearest_matchy + (int)(xx / zoom_x) * 4;
			dstpixels[0] = srcpixels[nearest_match];
			dstpixels[1] = srcpixels[nearest_match + 1];
			dstpixels[2] = srcpixels[nearest_match + 2];
			dstpixels[3] = srcpixels[nearest_match + 3];
			dstpixels += 4;
		}
	}

	SDL_UnlockSurface(surface);
	SDL_FreeSurface(bitmap);

	bitmap = surface;

	nbitmap->bitmap = NULL;
	delete nbitmap;

	Changed();*/
}

////////////////////////////////////////////////////////////
/// Resample
////////////////////////////////////////////////////////////
std::auto_ptr< Bitmap > Bitmap::Resample(int scalew, int scaleh, Rect const& srcRect) const
{
	double zoom_x = (double)(scalew) / srcRect.width;
	double zoom_y = (double)(scaleh) / srcRect.height;

	std::auto_ptr< Bitmap > resampled( new Bitmap(scalew, scaleh) );

	Uint8 const* src_pixels = (Uint8*)&pixels_[0];
	Uint8* dst_pixels = (Uint8*)(&resampled->pixels_[0]);

	int row = width_ * 4;

	for(int yy = 0; yy < scaleh; yy++) {
		int nearest_matchy = (srcRect.y + (int)(yy / zoom_y)) * row;
		for(int xx = 0; xx < scalew; xx++) {
			int nearest_match = nearest_matchy + (srcRect.x + (int)(xx / zoom_x)) * 4;
			dst_pixels[0] = src_pixels[nearest_match];
			dst_pixels[1] = src_pixels[nearest_match + 1];
			dst_pixels[2] = src_pixels[nearest_match + 2];
			dst_pixels[3] = src_pixels[nearest_match + 3];
			dst_pixels += 4;
		}
	}

	return resampled;
}

////////////////////////////////////////////////////////////
/// Rotate
////////////////////////////////////////////////////////////
void Bitmap::Rotate(float angle)
{
	/*while (angle > 360)
		angle -= 360;
	while (angle < 360)
		angle += 360;

	if (angle == 0) return;

	SDL_LockSurface(bitmap);

	float radians = angle * 3.14159f / 180.0f; 

	float cosine = (float)cos(radians); 
	float sine = (float)sin(radians); 

	float p1x = -getHeight() * sine;
	float p1y = getHeight() * cosine;
	float p2x = getWidth() * cosine - getHeight() * sine;
	float p2y = getHeight() * cosine + getWidth() * sine;
	float p3x = getWidth() * cosine;
	float p3y = getWidth() * sine;
	float std::minx = std::min(0, std::min(p1x, std::min(p2x, p3x))); 
	float std::miny = std::min(0, std::min(p1y, std::min(p2y, p3y))); 
	float std::maxx = std::max(p1x, std::max(p2x, p3x)); 
	float std::maxy = std::max(p1y, std::max(p2y, p3y)); 

	int nwidth = (int)ceil(fabs(std::maxx)-std::minx); 
	int nheight = (int)ceil(fabs(std::maxy)-std::miny);

	SDL_Surface* nbitmap = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, nwidth, nheight, 32, rmask, gmask, bmask, amask);

	SDL_LockSurface(nbitmap);

	Uint32* srcpixels = (Uint32*)bitmap->pixels_;
	Uint32* dstpixels = (Uint32*)nbitmap->pixels_;

	for(int i = 0; i < nbitmap->h; i++) {
		for(int j = 0; j < nbitmap->w; j++) {
			int sx = (int)((j + std::minx) * cosine + (i + std::miny) * sine); 
			int sy = (int)((i + std::miny) * cosine - (j + std::minx) * sine); 
			if (sx >= 0 && sx < bitmap->w && sy >= 0 && sy < bitmap->h) {
				dstpixels[0] = srcpixels[sy * bitmap->w + sx];
			}
			else {
				dstpixels[0] = 0;
			}
			dstpixels++;
		}
	}

	SDL_UnlockSurface(nbitmap);
	SDL_FreeSurface(bitmap);

	bitmap = nbitmap;

	Changed();*/
}
