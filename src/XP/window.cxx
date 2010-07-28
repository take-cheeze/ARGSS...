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
#include <string>
#include <cmath>
#include "window.hxx"
#include <argss/ruby.hxx>
#include <argss/window.hxx>
#include "graphics.hxx"
#include "viewport.hxx"
#include "player.hxx"
#include "rect.hxx"


////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////
Window::Window(VALUE iid)
: id_(iid)
, viewport_(rb_iv_get(id_, "@viewport"))
, windowskin_(Qnil)
, contents_(Qnil)
, stretch_(true)
, cursor_rect_(rb_iv_get(id_, "@cursor_rect"))
, active_(true)
, visible_(true)
, pause_(false)
, x_(0)
, y_(0)
, width_(0)
, height_(0)
, z_(0)
, ox_(0)
, oy_(0)
, opacity_(255)
, back_opacity_(255)
, contents_opacity_(255)
, cursor_frame_(0)
, pause_frame_(0)
, pause_id_(0)
{
	if ( ! NIL_P(viewport_) ) Viewport::get(viewport_).RegisterZObj(0, id_);
	else Graphics::RegisterZObj(0, id_);
}

////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Window::~Window()
{
}

////////////////////////////////////////////////////////////
/// Class Is Window Disposed?
////////////////////////////////////////////////////////////
bool Window::IsDisposed(VALUE id)
{
	return Graphics::countDrawable(id) == 0;
}

////////////////////////////////////////////////////////////
/// Class New Window
////////////////////////////////////////////////////////////
void Window::New(VALUE id)
{
	Graphics::insertDrawable(id, boost::shared_ptr< Drawable >( new Window(id) ) );
}

////////////////////////////////////////////////////////////
/// Class get Window
////////////////////////////////////////////////////////////
Window& Window::get(VALUE id)
{
	return dynamic_cast< Window& >( Graphics::getDrawable(id) );
}

////////////////////////////////////////////////////////////
/// Class Dispose Window
////////////////////////////////////////////////////////////
void Window::Dispose(VALUE id)
{
	if( ! NIL_P(Window::get(id).viewport_) ) Viewport::get(Window::get(id).viewport_).RemoveZObj(id);
	else Graphics::RemoveZObj(id);

	Graphics::eraseDrawable(id);
}

////////////////////////////////////////////////////////////
/// Refresh Bitmaps
////////////////////////////////////////////////////////////
void Window::RefreshBitmaps()
{
}

////////////////////////////////////////////////////////////
/// Draw
////////////////////////////////////////////////////////////
void Window::draw(long z)
{
	if (!visible_) return;
	if (width_ <= 0 || height_ <= 0) return;
	if (x_ < -width_ || x_ > Player::getWidth() || y_ < -height_ || y_ > Player::getHeight()) return;

	GLfloat texCoords[4][2];
	GLfloat vertexes[4][2];
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if ( ! NIL_P(viewport_) ) {
		Rect const& rect = Viewport::get(viewport_).getViewportRect();

		glEnable(GL_SCISSOR_TEST);
		glScissor(rect.x, Player::getHeight() - (rect.y + rect.height), rect.width, rect.height);

		glTranslatef((float)rect.x, (float)rect.y, 0.0f);
	}

	glTranslatef((GLfloat)x_, (GLfloat)y_, 0.0f);

	if ( ! NIL_P(windowskin_) ) {
		Bitmap& bmp = Bitmap::get(windowskin_);

		glPushMatrix();

		bmp.Refresh();
		bmp.BindBitmap();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		float bmpw = (float)bmp.getWidth(), bmph = (float)bmp.getHeight();
		float widthf = (float)width_, heightf = (float)height_;

	// Background
		if (width_ > 4 && height_ > 4 && (back_opacity_ * opacity_ / 255 > 0)) {
			glColor4f(1.0f, 1.0f, 1.0f, (back_opacity_ * opacity_ / 255.0f) / 255.0f);

			texCoords[0][0] = 0.0f; texCoords[0][1] = 0.0f;
			texCoords[1][0] = 128.0f / bmpw; texCoords[1][1] = 0.0f;
			texCoords[2][0] = 128.0f / bmpw; texCoords[2][1] = 128.0f / bmph;
			texCoords[3][0] = 0.0f; texCoords[3][1] = 128.0f / bmph;
			if (stretch_) {
				vertexes[0][0] = 2.0f; vertexes[0][1] = 2.0f;
				vertexes[1][0] = widthf - 4.0f; vertexes[1][1] = 2.0f;
				vertexes[2][0] = widthf - 4.0f; vertexes[2][1] = heightf - 4.0f;
				vertexes[3][0] = 2.0f; vertexes[3][1] = heightf - 4.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			} else {
				glEnable(GL_SCISSOR_TEST);

				Rect dstrect(x_ + 2, y_ + 2, width_ - 4, height_ - 4);

				if ( ! NIL_P(viewport_) ) {
					Rect const& rect = Viewport::get(viewport_).getViewportRect();

					dstrect.x -= rect.x;
					dstrect.y -= rect.y;

					dstrect.Adjust(rect.width, rect.height);
				}
				//glScissor(x + 2, Player::getHeight() - (y + 2 + height - 4), width - 4, height - 4);
				glScissor(dstrect.x, Player::getHeight() - (dstrect.y + dstrect.height), dstrect.width, dstrect.height);

				float tilesx = std::ceil(widthf / 128.0f);
				float tilesy = std::ceil(heightf / 128.0f);
				for (float x = 0; x < tilesx; x++) {
				for (float y = 0; y < tilesy; y++) {
					vertexes[0][0] = 2.0f + x * 128.0f; vertexes[0][1] = 2.0f + y * 128.0f;
					vertexes[1][0] = 2.0f + (x + 1) * 128.0f; vertexes[1][1] = 2.0f + y * 128.0f;
					vertexes[2][0] = 2.0f + (x + 1) * 128.0f; vertexes[2][1] = 2.0f + (y + 1) * 128.0f;
					vertexes[3][0] = 2.0f + x * 128.0f; vertexes[3][1] = 2.0f + (y + 1) * 128.0f;

					glVertexPointer(2, GL_FLOAT, 0, vertexes);
					glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
					glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
				}
				}
	
				if ( NIL_P(viewport_) ) glDisable(GL_SCISSOR_TEST);
			}
		}
	// Frame
		if (width_ > 0 && height_ > 0 && opacity_ > 0) {
			glColor4f(1.0f, 1.0f, 1.0f, opacity_ / 255.0f);

		// Corner upper left
			texCoords[0][0] = 128.0f / bmpw; texCoords[0][1] = 0.0f; vertexes[0][0] = 0.0f; vertexes[0][1] = 0.0f;
			texCoords[1][0] = 144.0f / bmpw; texCoords[1][1] = 0.0f; vertexes[1][0] = 16.0f; vertexes[1][1] = 0.0f;
			texCoords[2][0] = 144.0f / bmpw; texCoords[2][1] = 16.0f / bmph; vertexes[2][0] = 16.0f; vertexes[2][1] = 16.0f;
			texCoords[3][0] = 128.0f / bmpw; texCoords[3][1] = 16.0f / bmph; vertexes[3][0] =  0.0f; vertexes[3][1] = 16.0f;

			glVertexPointer(2, GL_FLOAT, 0, vertexes);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		// Corner upper right
			texCoords[0][0] = 176.0f / bmpw; texCoords[0][1] = 0.0f; vertexes[0][0] = widthf - 16.0f; vertexes[0][1] = 0.0f;
			texCoords[1][0] = 192.0f / bmpw; texCoords[1][1] = 0.0f; vertexes[1][0] = widthf; vertexes[1][1] = 0.0f;
			texCoords[2][0] = 192.0f / bmpw; texCoords[2][1] = 16.0f / bmph; vertexes[2][0] = widthf; vertexes[2][1] = 16.0f;
			texCoords[3][0] = 176.0f / bmpw; texCoords[3][1] = 16.0f / bmph; vertexes[3][0] = widthf - 16.0f; vertexes[3][1] = 16.0f;

			glVertexPointer(2, GL_FLOAT, 0, vertexes);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		// Corner lower right
			texCoords[0][0] = 176.0f / bmpw; texCoords[0][1] = 48.0f / bmph; vertexes[0][0] = widthf - 16.0f; vertexes[0][1] = heightf - 16.0f;
			texCoords[1][0] = 192.0f / bmpw; texCoords[1][1] = 48.0f / bmph; vertexes[1][0] = widthf; vertexes[1][1] = heightf - 16.0f;
			texCoords[2][0] = 192.0f / bmpw; texCoords[2][1] = 64.0f / bmph; vertexes[2][0] = widthf; vertexes[2][1] = heightf;
			texCoords[3][0] = 176.0f / bmpw; texCoords[3][1] = 64.0f / bmph; vertexes[3][0] = widthf - 16.0f; vertexes[3][1] = heightf;

			glVertexPointer(2, GL_FLOAT, 0, vertexes);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		// Corner lower left
			texCoords[0][0] = 128.0f / bmpw; texCoords[0][1] = 48.0f / bmph; vertexes[0][0] =  0.0f; vertexes[0][1] = heightf - 16.0f;
			texCoords[1][0] = 144.0f / bmpw; texCoords[1][1] = 48.0f / bmph; vertexes[1][0] = 16.0f; vertexes[1][1] = heightf - 16.0f;
			texCoords[2][0] = 144.0f / bmpw; texCoords[2][1] = 64.0f / bmph; vertexes[2][0] = 16.0f; vertexes[2][1] = heightf;
			texCoords[3][0] = 128.0f / bmpw; texCoords[3][1] = 64.0f / bmph; vertexes[3][0] =  0.0f; vertexes[3][1] = heightf;

			glVertexPointer(2, GL_FLOAT, 0, vertexes);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		// Border up
			texCoords[0][0] = 144.0f / bmpw; texCoords[0][1] = 0.0f; vertexes[0][0] = 16.0f; vertexes[0][1] = 0.0f;
			texCoords[1][0] = 176.0f / bmpw; texCoords[1][1] = 0.0f; vertexes[1][0] = std::max(widthf - 16.0f, 1.0f); vertexes[1][1] = 0.0f;
			texCoords[2][0] = 176.0f / bmpw; texCoords[2][1] = 16.0f / bmph; vertexes[2][0] = std::max(widthf - 16.0f, 1.0f); vertexes[2][1] = 16.0f;
			texCoords[3][0] = 144.0f / bmpw; texCoords[3][1] = 16.0f / bmph; vertexes[3][0] = 16.0f; vertexes[3][1] = 16.0f;

			glVertexPointer(2, GL_FLOAT, 0, vertexes);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		// Border down
			texCoords[0][0] = 144.0f / bmpw; texCoords[0][1] = 48.0f / bmph; vertexes[0][0] = 16.0f; vertexes[0][1] = heightf - 16.0f;
			texCoords[1][0] = 176.0f / bmpw; texCoords[1][1] = 48.0f / bmph; vertexes[1][0] = std::max(widthf - 16.0f, 1.0f); vertexes[1][1] = heightf - 16.0f;
			texCoords[2][0] = 176.0f / bmpw; texCoords[2][1] = 64.0f / bmph; vertexes[2][0] = std::max(widthf - 16.0f, 1.0f); vertexes[2][1] = heightf;
			texCoords[3][0] = 144.0f / bmpw; texCoords[3][1] = 64.0f / bmph; vertexes[3][0] = 16.0f; vertexes[3][1] = heightf;

			glVertexPointer(2, GL_FLOAT, 0, vertexes);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		// Border left
			texCoords[0][0] = 128.0f / bmpw; texCoords[0][1] = 16.0f / bmph; vertexes[0][0] =  0.0f; vertexes[0][1] = 16.0f;
			texCoords[1][0] = 144.0f / bmpw; texCoords[1][1] = 16.0f / bmph; vertexes[1][0] = 16.0f; vertexes[1][1] = 16.0f;
			texCoords[2][0] = 144.0f / bmpw; texCoords[2][1] = 48.0f / bmph; vertexes[2][0] = 16.0f; vertexes[2][1] = std::max(heightf - 16.0f, 1.0f);
			texCoords[3][0] = 128.0f / bmpw; texCoords[3][1] = 48.0f / bmph; vertexes[3][0] =  0.0f; vertexes[3][1] = std::max(heightf - 16.0f, 1.0f);

			glVertexPointer(2, GL_FLOAT, 0, vertexes);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		// Border right
			texCoords[0][0] = 176.0f / bmpw; texCoords[0][1] = 16.0f / bmph; vertexes[0][0] = widthf - 16.0f; vertexes[0][1] = 16.0f;
			texCoords[1][0] = 192.0f / bmpw; texCoords[1][1] = 16.0f / bmph; vertexes[1][0] = widthf; vertexes[1][1] = 16.0f;
			texCoords[2][0] = 192.0f / bmpw; texCoords[2][1] = 48.0f / bmph; vertexes[2][0] = widthf; vertexes[2][1] = std::max(heightf - 16.0f, 1.0f);
			texCoords[3][0] = 176.0f / bmpw; texCoords[3][1] = 48.0f / bmph; vertexes[3][0] = widthf - 16.0f; vertexes[3][1] = std::max(heightf - 16.0f, 1.0f);

			glVertexPointer(2, GL_FLOAT, 0, vertexes);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}

		// Cursor
		if (width_ > 32 && height_ > 32) {
			Rect rect(cursor_rect_);
			if (rect.width > 0 && rect.height > 0) {
				float cursor_opacity = 255.0f;
				cursor_opacity -= (92.0f / 16.0f) * ( (cursor_frame_ <= 16) ? cursor_frame_ : (32 - cursor_frame_) );
				glColor4f(1.0f, 1.0f, 1.0f, cursor_opacity / 255.0f);

				float left = (float)rect.x + 16;
				float top = (float)rect.y + 16;
				float rigth = left + rect.width;
				float bottom = top + rect.height;

			// Background
				texCoords[0][0] = 130.0f / bmpw; texCoords[0][1] = 66.0f / bmph; vertexes[0][0] = left + 2.0f; vertexes[0][1] = top + 2.0f;
				texCoords[1][0] = 158.0f / bmpw; texCoords[1][1] = 66.0f / bmph; vertexes[1][0] = rigth - 2.0f; vertexes[1][1] = top + 2.0f;
				texCoords[2][0] = 158.0f / bmpw; texCoords[2][1] = 94.0f / bmph; vertexes[2][0] = rigth - 2.0f; vertexes[2][1] = bottom - 2.0f;
				texCoords[3][0] = 130.0f / bmpw; texCoords[3][1] = 94.0f / bmph; vertexes[3][0] = left + 2.0f; vertexes[3][1] = bottom - 2.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			// Corner upper left
				texCoords[0][0] = 128.0f / bmpw; texCoords[0][1] = 64.0f / bmph; vertexes[0][0] = left; vertexes[0][1] = top;
				texCoords[1][0] = 130.0f / bmpw; texCoords[1][1] = 64.0f / bmph; vertexes[1][0] = left + 2.0f; vertexes[1][1] = top;
				texCoords[2][0] = 130.0f / bmpw; texCoords[2][1] = 66.0f / bmph; vertexes[2][0] = left + 2.0f; vertexes[2][1] = top + 2.0f;
				texCoords[3][0] = 128.0f / bmpw; texCoords[3][1] = 66.0f / bmph; vertexes[3][0] = left; vertexes[3][1] = top + 2.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			// Corner upper right
				texCoords[0][0] = 158.0f / bmpw; texCoords[0][1] = 64.0f / bmph; vertexes[0][0] = rigth - 2.0f; vertexes[0][1] = top;
				texCoords[1][0] = 160.0f / bmpw; texCoords[1][1] = 64.0f / bmph; vertexes[1][0] = rigth; vertexes[1][1] = top;
				texCoords[2][0] = 160.0f / bmpw; texCoords[2][1] = 66.0f / bmph; vertexes[2][0] = rigth; vertexes[2][1] = top + 2.0f;
				texCoords[3][0] = 158.0f / bmpw; texCoords[3][1] = 66.0f / bmph; vertexes[3][0] = rigth - 2.0f; vertexes[3][1] = top + 2.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			// Corner lower right
				texCoords[0][0] = 158.0f / bmpw; texCoords[0][1] = 94.0f / bmph; vertexes[0][0] = rigth - 2.0f; vertexes[0][1] = bottom - 2.0f;
				texCoords[1][0] = 160.0f / bmpw; texCoords[1][1] = 94.0f / bmph; vertexes[1][0] = rigth; vertexes[1][1] = bottom - 2.0f;
				texCoords[2][0] = 160.0f / bmpw; texCoords[2][1] = 96.0f / bmph; vertexes[2][0] = rigth; vertexes[2][1] = bottom;
				texCoords[3][0] = 158.0f / bmpw; texCoords[3][1] = 96.0f / bmph; vertexes[3][0] = rigth - 2.0f; vertexes[3][1] = bottom;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			// Corner lower left
				texCoords[0][0] = 128.0f / bmpw; texCoords[0][1] = 94.0f / bmph; vertexes[0][0] = left; vertexes[0][1] = bottom - 2.0f;
				texCoords[1][0] = 130.0f / bmpw; texCoords[1][1] = 94.0f / bmph; vertexes[1][0] = left + 2.0f; vertexes[1][1] = bottom - 2.0f;
				texCoords[2][0] = 130.0f / bmpw; texCoords[2][1] = 96.0f / bmph; vertexes[2][0] = left + 2.0f; vertexes[2][1] = bottom;
				texCoords[3][0] = 128.0f / bmpw; texCoords[3][1] = 96.0f / bmph; vertexes[3][0] = left; vertexes[3][1] = bottom;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			// Border up
				texCoords[0][0] = 130.0f / bmpw; texCoords[0][1] = 64.0f / bmph; vertexes[0][0] = left  + 2.0f; vertexes[0][1] = top;
				texCoords[1][0] = 158.0f / bmpw; texCoords[1][1] = 64.0f / bmph; vertexes[1][0] = rigth - 2.0f; vertexes[1][1] = top;
				texCoords[2][0] = 158.0f / bmpw; texCoords[2][1] = 66.0f / bmph; vertexes[2][0] = rigth - 2.0f; vertexes[2][1] = top + 2.0f;
				texCoords[3][0] = 130.0f / bmpw; texCoords[3][1] = 66.0f / bmph; vertexes[3][0] = left  + 2.0f; vertexes[3][1] = top + 2.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			// Border down
				texCoords[0][0] = 130.0f / bmpw; texCoords[0][1] = 94.0f / bmph; vertexes[0][0] = left  + 2.0f; vertexes[0][1] = bottom - 2.0f;
				texCoords[1][0] = 158.0f / bmpw; texCoords[1][1] = 94.0f / bmph; vertexes[1][0] = rigth - 2.0f; vertexes[1][1] = bottom - 2.0f;
				texCoords[2][0] = 158.0f / bmpw; texCoords[2][1] = 96.0f / bmph; vertexes[2][0] = rigth - 2.0f; vertexes[2][1] = bottom;
				texCoords[3][0] = 130.0f / bmpw; texCoords[3][1] = 96.0f / bmph; vertexes[3][0] = left  + 2.0f; vertexes[3][1] = bottom;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			// Border left
				texCoords[0][0] = 128.0f / bmpw; texCoords[0][1] = 66.0f / bmph; vertexes[0][0] = left; vertexes[0][1] = top + 2.0f;
				texCoords[1][0] = 130.0f / bmpw; texCoords[1][1] = 66.0f / bmph; vertexes[1][0] = left + 2.0f; vertexes[1][1] = top + 2.0f;
				texCoords[2][0] = 130.0f / bmpw; texCoords[2][1] = 94.0f / bmph; vertexes[2][0] = left + 2.0f; vertexes[2][1] = bottom - 2.0f;
				texCoords[3][0] = 128.0f / bmpw; texCoords[3][1] = 94.0f / bmph; vertexes[3][0] = left; vertexes[3][1] = bottom - 2.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			// Border right
				texCoords[0][0] = 158.0f / bmpw; texCoords[0][1] = 66.0f / bmph; vertexes[0][0] = rigth - 2.0f; vertexes[0][1] = top + 2.0f;
				texCoords[1][0] = 160.0f / bmpw; texCoords[1][1] = 66.0f / bmph; vertexes[1][0] = rigth; vertexes[1][1] = top + 2.0f;
				texCoords[2][0] = 160.0f / bmpw; texCoords[2][1] = 94.0f / bmph; vertexes[2][0] = rigth; vertexes[2][1] = bottom - 2.0f;
				texCoords[3][0] = 158.0f / bmpw; texCoords[3][1] = 94.0f / bmph; vertexes[3][0] = rigth - 2.0f; vertexes[3][1] = bottom - 2.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
		}
		if (pause_) {
			float dstx = (float)std::max(width_ / 2 - 8, 0), dsty = (float)std::max(height_ - 16, 0);
			float w = (float)std::min(16, width_), h = (float)std::min(16, height_);
			float srcx = 176.0f, srcy = 64.0f;
			switch (pause_id_) {
				case 0:
					glColor4f(1.0f, 1.0f, 1.0f, (255.0f / 8.0f) * pause_frame_ / 255.0f);
				case 4: srcx = 160.0f; srcy = 64.0f; break;
				case 1: srcx = 176.0f; srcy = 64.0f; break;
				case 2: srcx = 160.0f; srcy = 80.0f; break;
				case 3: srcx = 176.0f; srcy = 80.0f; break;
			}
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			texCoords[0][0] = srcx / bmpw; texCoords[0][1] = srcy / bmph; vertexes[0][0] = dstx; vertexes[0][1] = dsty;
			texCoords[1][0] = (srcx + w) / bmpw; texCoords[1][1] = srcy / bmph; vertexes[1][0] = dstx + w; vertexes[1][1] = dsty;
			texCoords[2][0] = (srcx + w) / bmpw; texCoords[2][1] = (srcy + h) / bmph; vertexes[2][0] = dstx + w; vertexes[2][1] = dsty + h;
			texCoords[3][0] = srcx / bmpw; texCoords[3][1] = (srcy + h) / bmph; vertexes[3][0] = dstx; vertexes[3][1] = dsty + h;

			glVertexPointer(2, GL_FLOAT, 0, vertexes);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
	}

	if ( ! NIL_P(contents_) ) {
		if (width_ > 32 && height_ > 32 && -ox_ < width_ - 32 && -oy_ < height_ - 32 && contents_opacity_ > 0) {
			Bitmap& bmp = Bitmap::get(contents_);

			glPushMatrix();

			bmp.Refresh();
			bmp.BindBitmap();

			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();

			Rect dstrect(x_ + 16, y_ + 16, width_ - 32, height_ - 32);

			glEnable(GL_SCISSOR_TEST);
			if ( ! NIL_P(viewport_) ) {
				Rect rect = Viewport::get(viewport_).getViewportRect();

				dstrect.x -= rect.x;
				dstrect.y -= rect.y;

				dstrect.Adjust(rect.width, rect.height);
			}

			glScissor(dstrect.x, Player::getHeight() - (dstrect.y + dstrect.height), dstrect.width, dstrect.height);

			glColor4f(1.0f, 1.0f, 1.0f, contents_opacity_ / 255.0f);

			texCoords[0][0] = 0.0f; texCoords[0][1] = 0.0f; vertexes[0][0] = 16.0f - ox_; vertexes[0][1] = 16.0f - oy_;
			texCoords[1][0] = 1.0f; texCoords[1][1] = 0.0f; vertexes[1][0] = 16.0f - ox_ + bmp.getWidth(); vertexes[1][1] = 16.0f - oy_;
			texCoords[2][0] = 1.0f; texCoords[2][1] = 1.0f; vertexes[2][0] = 16.0f - ox_ + bmp.getWidth(); vertexes[2][1] = 16.0f - oy_ + bmp.getHeight();
			texCoords[3][0] = 0.0f; texCoords[3][1] = 1.0f; vertexes[3][0] = 16.0f - ox_; vertexes[3][1] = 16.0f - oy_ + bmp.getHeight();

			glVertexPointer(2, GL_FLOAT, 0, vertexes);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			glViewport(0, 0, Player::getWidth(), Player::getHeight());

			if ( NIL_P(viewport_) ) glDisable(GL_SCISSOR_TEST);
		}

		if ( ! NIL_P(windowskin_) ) {
			Bitmap& bmp = Bitmap::get(windowskin_);

			float bmpw = (float)bmp.getWidth();
			float bmph = (float)bmp.getHeight();
			float widthf = (float)width_;
			float heightf = (float)height_;

			bmp.BindBitmap();

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			if (ox_ > 0) {
				texCoords[0][0] = 144.0f / bmpw; texCoords[0][1] = 24.0f / bmph; vertexes[0][0] =  4.0f; vertexes[0][1] = heightf / 2.0f - 8.0f;
				texCoords[1][0] = 152.0f / bmpw; texCoords[1][1] = 24.0f / bmph; vertexes[1][0] = 12.0f; vertexes[1][1] = heightf / 2.0f - 8.0f;
				texCoords[2][0] = 152.0f / bmpw; texCoords[2][1] = 40.0f / bmph; vertexes[2][0] = 12.0f; vertexes[2][1] = heightf / 2.0f + 8.0f;
				texCoords[3][0] = 144.0f / bmpw; texCoords[3][1] = 40.0f / bmph; vertexes[3][0] =  4.0f; vertexes[3][1] = heightf / 2.0f + 8.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
			if (oy_ > 0) {
				texCoords[0][0] = 152.0f / bmpw; texCoords[0][1] = 16.0f / bmph; vertexes[0][0] = widthf / 2.0f - 8.0f; vertexes[0][1] =  4.0f;
				texCoords[1][0] = 168.0f / bmpw; texCoords[1][1] = 16.0f / bmph; vertexes[1][0] = widthf / 2.0f + 8.0f; vertexes[1][1] =  4.0f;
				texCoords[2][0] = 168.0f / bmpw; texCoords[2][1] = 24.0f / bmph; vertexes[2][0] = widthf / 2.0f + 8.0f; vertexes[2][1] = 12.0f;
				texCoords[3][0] = 152.0f / bmpw; texCoords[3][1] = 24.0f / bmph; vertexes[3][0] = widthf / 2.0f - 8.0f; vertexes[3][1] = 12.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
			if (Bitmap::get(contents_).getWidth() - ox_ > width_ - 32) {
				texCoords[0][0] = 168.0f / bmpw; texCoords[0][1] = 24.0f / bmph; vertexes[0][0] = widthf - 12.0f; vertexes[0][1] = heightf / 2.0f - 8.0f;
				texCoords[1][0] = 176.0f / bmpw; texCoords[1][1] = 24.0f / bmph; vertexes[1][0] = widthf -  4.0f; vertexes[1][1] = heightf / 2.0f - 8.0f;
				texCoords[2][0] = 176.0f / bmpw; texCoords[2][1] = 40.0f / bmph; vertexes[2][0] = widthf -  4.0f; vertexes[2][1] = heightf / 2.0f + 8.0f;
				texCoords[3][0] = 168.0f / bmpw; texCoords[3][1] = 40.0f / bmph; vertexes[3][0] = widthf - 12.0f; vertexes[3][1] = heightf / 2.0f + 8.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
			if (Bitmap::get(contents_).getHeight() - oy_ > height_ - 32) {
				texCoords[0][0] = 152.0f / bmpw; texCoords[0][1] = 40.0f / bmph; vertexes[0][0] = widthf / 2.0f - 8.0f; vertexes[0][1] = heightf - 12.0f;
				texCoords[1][0] = 168.0f / bmpw; texCoords[1][1] = 40.0f / bmph; vertexes[1][0] = widthf / 2.0f + 8.0f; vertexes[1][1] = heightf - 12.0f;
				texCoords[2][0] = 168.0f / bmpw; texCoords[2][1] = 48.0f / bmph; vertexes[2][0] = widthf / 2.0f + 8.0f; vertexes[2][1] = heightf -  4.0f;
				texCoords[3][0] = 152.0f / bmpw; texCoords[3][1] = 48.0f / bmph; vertexes[3][0] = widthf / 2.0f - 8.0f; vertexes[3][1] = heightf -  4.0f;

				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_SCISSOR_TEST);
}
void Window::draw(long z, Bitmap const& dst_bitmap)
{
}

////////////////////////////////////////////////////////////
/// Update
////////////////////////////////////////////////////////////
void Window::Update()
{
	if (active_) {
		cursor_frame_ += 1;
		if (cursor_frame_ == 32) cursor_frame_ = 0;
		if (pause_) {
			pause_frame_ += 1;
			if (pause_frame_ == 8) {
				pause_frame_ = 0;
				pause_id_ += 1;
				if (pause_id_ == 5) pause_id_ = 1;
			}
		}
	}
}

////////////////////////////////////////////////////////////
/// Properties
////////////////////////////////////////////////////////////
void Window::setViewport(VALUE nviewport)
{
	if(viewport_ != nviewport) {
		if( ! NIL_P(nviewport) ) {
			Graphics::RemoveZObj(id_);
			Viewport::get(nviewport).RegisterZObj(0, id_);
		} else {
			if ( ! NIL_P(viewport_) ) Viewport::get(viewport_).RemoveZObj(id_);
			Graphics::RegisterZObj(0, id_);
		}
	}
	viewport_ = nviewport;
}
void Window::setZ(int nz)
{
	if(z_ != nz) {
		if( ! NIL_P(viewport_) ) Viewport::get(viewport_).UpdateZObj(id_, nz);
		else Graphics::UpdateZObj(id_, nz);
	}
	z_ = nz;
}
