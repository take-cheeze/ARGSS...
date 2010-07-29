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
#include <cstring>

#include <string>

#include <GL/gl.h>

#include <argss/ruby.hxx>
#include <argss/sprite.hxx>

#include "graphics.hxx"
#include "player.hxx"
#include "sprite.hxx"
#include "system.hxx"
#include "viewport.hxx"

////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////
Sprite::Sprite(VALUE iid)
: id(iid)
, viewport_( rb_iv_get(id, "@viewport") )
, bitmap_(Qnil), srcRect_( rb_iv_get(id, "@src_rect") )
, visible(true)
, x(0), y(0), z(0), ox(0), oy(0)
, zoom_x(1.0), zoom_y(1.0)
, angle(0), flipx(false), flipy(false)
, bush_depth(0), bush_opacity(128), opacity(255)
, blendType_(Blend::NORMAL)
, color( rb_iv_get(id, "@color") )
, tone( rb_iv_get(id, "@tone") )
, flash_duration(0), sprite_()
, flash_needs_refresh(false)
{
	if ( ! NIL_P(viewport_) ) Viewport::get(viewport_).RegisterZObj(0, id);
	else Graphics::RegisterZObj(0, id);
}

////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Sprite::~Sprite()
{
	if (flash_texture > 0) {
		glDeleteTextures(1, &flash_texture);
		flash_texture = 0;
	}
}

////////////////////////////////////////////////////////////
/// Class Is Sprite Disposed?
////////////////////////////////////////////////////////////
bool Sprite::IsDisposed(VALUE id)
{
	return Graphics::countDrawable(id) == 0;
}

////////////////////////////////////////////////////////////
/// Class New Sprite
////////////////////////////////////////////////////////////
void Sprite::New(VALUE id)
{
	Graphics::insertDrawable( id, boost::shared_ptr< Drawable >( new Sprite(id) ) );
}

////////////////////////////////////////////////////////////
/// Class get Sprite
////////////////////////////////////////////////////////////
Sprite& Sprite::get(VALUE id)
{
	return dynamic_cast< Sprite& >( Graphics::getDrawable(id) );
}

////////////////////////////////////////////////////////////
/// Class Dispose Sprite
////////////////////////////////////////////////////////////
void Sprite::Dispose(VALUE id)
{
	if ( ! NIL_P( Sprite::get(id).viewport_ ) ) Viewport::get(Sprite::get(id).viewport_).RemoveZObj(id);
	else Graphics::RemoveZObj(id);

	Graphics::eraseDrawable(id);
}

////////////////////////////////////////////////////////////
/// Refresh Bitmaps
////////////////////////////////////////////////////////////
void Sprite::RefreshBitmaps()
{
	if ( sprite_.get() ) sprite_->Changed();
}

////////////////////////////////////////////////////////////
/// Draw
////////////////////////////////////////////////////////////
void Sprite::draw(long z)
{
	if( !getVisible() || NIL_P(bitmap_) ) return;

	Rect srcRectSprite(srcRect_);

	int width = srcRectSprite.width;
	int height = srcRectSprite.height;
	if(width <= 0 || height <= 0)
	if(x < -width || x > Player::getWidth() || y < -height || y > Player::getHeight()) return;
	if(zoom_x == 0 || zoom_y == 0 || opacity == 0) return;
	if(flash_duration > 0 && flash_color.red == -1) return;

	Refresh();

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	sprite_->BindBitmap();

	glTranslatef((float)x, (float)y, 0.0f);

	if ( ! NIL_P(viewport_) ) {
		Rect rect = Viewport::get(viewport_).getViewportRect();

		glEnable(GL_SCISSOR_TEST);
		glScissor(rect.x, Player::getHeight() - (rect.y + rect.height), rect.width, rect.height);

		glTranslatef((float)rect.x, (float)rect.y, 0.0f);
	}

	glRotatef((float)angle, 0.0f, 0.0f, 1.0f);
	glTranslatef((float)-ox * zoom_x, (float)-oy * zoom_y, 0.0f);

	glColor4f(1.0f, 1.0f, 1.0f, opacity / 255.0f);

	GLfloat corners[4][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
	if (srcRectSprite != sprite_->getRect()) {
		float rx = (float)srcRectSprite.x / (float)sprite_->getWidth();
		float ry = (float)srcRectSprite.y / (float)sprite_->getHeight();
		float rw = (float)srcRectSprite.width / (float)sprite_->getWidth();
		float rh = (float)srcRectSprite.height / (float)sprite_->getHeight();
		corners[0][0] = rx; 		corners[0][1] = ry;
		corners[1][0] = rx + rw; 	corners[1][1] = ry;
		corners[2][0] = rx + rw; 	corners[2][1] = ry + rh;
		corners[3][0] = rx; 		corners[3][1] = ry + rh;
	}
	if (flipx && flipy) {
		corners[0][0] = 1.0f - corners[0][0]; corners[0][1] = 1.0f - corners[0][1];
		corners[1][0] = 1.0f - corners[1][0]; corners[1][1] = 1.0f - corners[1][1];
		corners[2][0] = 1.0f - corners[2][0]; corners[2][1] = 1.0f - corners[2][1];
		corners[3][0] = 1.0f - corners[3][0]; corners[3][1] = 1.0f - corners[3][1];
	} else if (flipx) {
		corners[0][0] = 1.0f - corners[0][0];
		corners[1][0] = 1.0f - corners[1][0];
		corners[2][0] = 1.0f - corners[2][0];
		corners[3][0] = 1.0f - corners[3][0];
	} else if (flipy) {
		corners[0][1] = 1.0f - corners[0][1];
		corners[1][1] = 1.0f - corners[1][1];
		corners[2][1] = 1.0f - corners[2][1];
		corners[3][1] = 1.0f - corners[3][1];
	}

	glEnable(GL_BLEND);
	setBlendFunc(blendType_);

	GLfloat texCoords[4][2]; GLfloat vertexes[4][2];
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	if (bush_depth == 0) {
		vertexes[0][0] = 0.f; vertexes[0][1] = 0.f;
		vertexes[1][0] = width * zoom_x; vertexes[1][1] = 0.f;
		vertexes[2][0] = width * zoom_x; vertexes[2][1] = height * zoom_y;
		vertexes[3][0] = 0.f; vertexes[3][1] = height * zoom_y;

		glVertexPointer(2, GL_FLOAT, 0, vertexes);
		glTexCoordPointer(2, GL_FLOAT, 0, corners);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	} else {
		int target;

		std::memcpy( vertexes, corners, sizeof(vertexes) );
		vertexes[0][0] = 0.f; vertexes[0][1] = 0.f;
		vertexes[1][0] = width * zoom_x; vertexes[1][1] = 0.f;
		vertexes[2][0] = width * zoom_x; vertexes[2][1] = height * zoom_y;
		vertexes[3][0] = 0.f; vertexes[3][1] = height * zoom_y;

		target = flipy ? 0 : 2;
		texCoords[target + 0][1] -= bush_depth / (float)sprite_->getHeight(); vertexes[target + 0][1] += bush_depth * zoom_y;
		texCoords[target + 1][1] -= bush_depth / (float)sprite_->getHeight(); vertexes[target + 1][1] += bush_depth * zoom_y;

		glVertexPointer(2, GL_FLOAT, 0, vertexes);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glColor4f(1.0f, 1.0f, 1.0f, (bush_opacity / 255.0f) * (opacity / 255.0f));

		std::memcpy( vertexes, corners, sizeof(vertexes) );
		vertexes[0][0] = 0.f; vertexes[0][1] = 0.f;
		vertexes[1][0] = width * zoom_x; vertexes[1][1] = 0.f;
		vertexes[2][0] = width * zoom_x; vertexes[2][1] = height * zoom_y;
		vertexes[3][0] = 0.f; vertexes[3][1] = height * zoom_y;

		target = flipy ? 2 : 0;
		texCoords[target + 0][1] -= bush_depth / (float)sprite_->getHeight(); vertexes[target + 0][1] += bush_depth * zoom_y;
		texCoords[target + 1][1] -= bush_depth / (float)sprite_->getHeight(); vertexes[target + 1][1] += bush_depth * zoom_y;

		glVertexPointer(2, GL_FLOAT, 0, vertexes);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	if (flash_duration > 0) {
		glBindTexture(GL_TEXTURE_2D, flash_texture);
		GLfloat alpha = ((float)flash_color.alpha / 255.0f) * (1.0f - flash_frame / (float)flash_duration);
		glColor4f((float)flash_color.red / 255.0f, (GLfloat)flash_color.green / 255.0f, (float)flash_color.blue / 255.0f, alpha);

		vertexes[0][0] = 0.f; vertexes[0][1] = 0.f;
		vertexes[1][0] = width * zoom_x; vertexes[1][1] = 0.f;
		vertexes[2][0] = width * zoom_x; vertexes[2][1] = height * zoom_y;
		vertexes[3][0] = 0.f; vertexes[3][1] = height * zoom_y;

		glVertexPointer(2, GL_FLOAT, 0, vertexes);
		glTexCoordPointer(2, GL_FLOAT, 0, corners);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_SCISSOR_TEST);
	/*if ( !getVisible() ) return;
	if ( NIL_P(bitmap_) ) return;

	srcRectSprite = Rect(srcRect_);

	int width = srcRectSprite.width;
	int height = srcRectSprite.height;
	if (width <= 0 || height <= 0)
	if (x < -width || x > Player::getWidth() || y < -height || y > Player::getHeight()) return;
	if (zoom_x == 0 || zoom_y == 0 || opacity == 0) return;
	if (flash_duration > 0 && flash_color.red == -1) return;

	Refresh();

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	sprite->BindBitmap();

	glTranslatef((float)x, (float)y, 0.0f);

	if ( ! NIL_P(viewport_) ) {
		Rect rect = Viewport::get(viewport_)->getViewportRect();

		glEnable(GL_SCISSOR_TEST);
		glScissor(rect.x, Player::getHeight() - (rect.y + rect.height), rect.width, rect.height);

		glTranslatef((float)rect.x, (float)rect.y, 0.0f);
	}

	glRotatef((float)angle, 0.0f, 0.0f, 1.0f);
	glTranslatef((float)-ox * zoom_x, (float)-oy * zoom_y, 0.0f);

	glColor4f(1.0f, 1.0f, 1.0f, opacity / 255.0f);

	float corners[4][2] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
	if (flipx && flipy) {
		corners[0][0] = 1.0f; corners[0][1] = 1.0f;
		corners[1][0] = 0.0f; corners[1][1] = 1.0f;
		corners[2][0] = 0.0f; corners[2][1] = 0.0f;
		corners[3][0] = 1.0f; corners[3][1] = 0.0f;
	}
	else if (flipx) {
		corners[0][0] = 1.0f; corners[0][1] = 0.0f;
		corners[1][0] = 0.0f; corners[1][1] = 0.0f;
		corners[2][0] = 0.0f; corners[2][1] = 1.0f;
		corners[3][0] = 1.0f; corners[3][1] = 1.0f;
	}
	else if (flipy) {
		corners[0][0] = 0.0f; corners[0][1] = 1.0f;
		corners[1][0] = 1.0f; corners[1][1] = 1.0f;
		corners[2][0] = 1.0f; corners[2][1] = 0.0f;
		corners[3][0] = 0.0f; corners[3][1] = 0.0f;
	}

	glEnable(GL_BLEND);
	setBlendFunc(blendType_);

	if (bush_depth == 0) {
		glBegin(GL_QUADS);
			glTexCoord2f(corners[0][0], corners[0][1]); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(corners[1][0], corners[1][1]); glVertex2f(width * zoom_x, 0.0f);
			glTexCoord2f(corners[2][0], corners[2][1]); glVertex2f(width * zoom_x, height  * zoom_y);
			glTexCoord2f(corners[3][0], corners[3][1]); glVertex2f(0.0f, height * zoom_y);
		glEnd();
	}
	else {
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); 								glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); 								glVertex2f(width * zoom_x, 0.0f);
			glTexCoord2f(1.0f, 1.0f - bush_depth / (float)height);	glVertex2f(width * zoom_x, (height - bush_depth)  * zoom_y);
			glTexCoord2f(0.0f, 1.0f - bush_depth / (float)height);  glVertex2f(0.0f, (height - bush_depth) * zoom_y);
		glEnd();

		glColor4f(1.0f, 1.0f, 1.0f, 0.5f * (opacity / 255.0f));
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, (1.0f - bush_depth / (float)height)); glVertex2f(0.0f, (height - bush_depth)  * zoom_y);
			glTexCoord2f(1.0f, (1.0f - bush_depth / (float)height)); glVertex2f(width * zoom_x, (height - bush_depth)  * zoom_y);
			glTexCoord2f(1.0f, 1.0f);								 glVertex2f(width * zoom_x, height  * zoom_y);
			glTexCoord2f(0.0f, 1.0f);								 glVertex2f(0.0f, height * zoom_y);
		glEnd();
	}

	if (flash_duration > 0) {
		glBindTexture(GL_TEXTURE_2D, flash_texture);
		GLfloat alpha = ((float)flash_color.alpha / 255.0f) * (1.0f - flash_frame / (float)flash_duration);
		glColor4f((float)flash_color.red / 255.0f, (GLfloat)flash_color.green / 255.0f, (float)flash_color.blue / 255.0f, alpha);
		glBegin(GL_QUADS);
			glTexCoord2f(corners[0][0], corners[0][1]); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(corners[1][0], corners[1][1]); glVertex2f(width * zoom_x, 0.0f);
			glTexCoord2f(corners[2][0], corners[2][1]); glVertex2f(width * zoom_x, height  * zoom_y);
			glTexCoord2f(corners[3][0], corners[3][1]); glVertex2f(0.0f, height * zoom_y);
		glEnd();
	}

	glDisable(GL_SCISSOR_TEST);*/
}
void Sprite::draw(long z, Bitmap const& dst_bitmap)
{
	/*if ( !getVisible() ) return;
	if (getWidth() <= 0 || getHeight() <= 0) return;
	if (x < -getWidth() || x > dst_bitmap->getWidth() || y < -getHeight() || y > dst_bitmap->getHeight()) return;
	if (!bitmap_) return;
	
	srcRectSprite = Rect(srcRect_);
	if (srcRectSprite != srcRect_last) {
		srcRect_last = srcRectSprite;
		needs_refresh = true;
	}
	
	if (needs_refresh) Refresh();
	
	dst_bitmap->Blit(x - ox, y - oy, sprite, sprite->getRect(), 255);*/
}

////////////////////////////////////////////////////////////
/// Refresh
////////////////////////////////////////////////////////////
void Sprite::Refresh()
{
	if (needs_refresh) {
		needs_refresh = false;

		sprite_.reset( new Bitmap( Bitmap::get(bitmap_), Bitmap::get(bitmap_).getRect() ) );

		sprite_->ToneChange(Tone(tone));
	}
	if (flash_needs_refresh) {
		flash_needs_refresh = false;
		RefreshFlash();
	}

	sprite_->Refresh();
	/*Tone tone_sprite(tone);

	if (srcRectSprite != srcRect_last) {
		srcRect_last = srcRectSprite;
		if (tone_sprite.red != 0 || tone_sprite.green != 0 || tone_sprite.blue != 0 || tone_sprite.gray != 0) {
			needs_refresh = true;
		}
		flash_needs_refresh = true;
	}

	if (needs_refresh) {
		needs_refresh = false;

		if (sprite) delete sprite;

		sprite = new Bitmap(Bitmap::get(bitmap_), srcRectSprite);

		sprite->ToneChange(tone_sprite);
	}
	if (flash_needs_refresh) {
		flash_needs_refresh = false;
		RefreshFlash();
	}

	sprite->Refresh();*/
}

////////////////////////////////////////////////////////////
/// get Width
////////////////////////////////////////////////////////////
int Sprite::getWidth()
{
	return NUM2INT(rb_iv_get(srcRect_, "@width"));
}

////////////////////////////////////////////////////////////
/// get Height
////////////////////////////////////////////////////////////
int Sprite::getHeight()
{
	return NUM2INT(rb_iv_get(srcRect_, "@height"));
}

////////////////////////////////////////////////////////////
/// Update
////////////////////////////////////////////////////////////
void Sprite::Update()
{
	if (flash_duration != 0) {
		flash_frame += 1;
		if (flash_duration == flash_frame) {
			flash_duration = 0;
			if (flash_texture > 0) {
				glDeleteTextures(1, &flash_texture);
				flash_texture = 0;
			}
		}
	}
}

////////////////////////////////////////////////////////////
/// Flash
////////////////////////////////////////////////////////////
void Sprite::Flash(int duration)
{
	flash_color = Color(-1, -1, -1, -1);
	flash_duration = duration;
	flash_frame = 0;
}
void Sprite::Flash(Color const& color, int duration)
{
	flash_color = color;
	flash_duration = duration;
	flash_frame = 0;
	flash_needs_refresh = true;
}
void Sprite::RefreshFlash()
{
	if (flash_texture > 0) glDeleteTextures(1, &flash_texture);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &flash_texture);
	glBindTexture(GL_TEXTURE_2D, flash_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelTransferf(GL_RED_BIAS, 255.0f);
	glPixelTransferf(GL_GREEN_BIAS, 255.0f);
	glPixelTransferf(GL_BLUE_BIAS, 255.0f);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, sprite_->getWidth(), sprite_->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite_->getPixels());

	glPixelTransferf(GL_RED_BIAS, 0.0f);
	glPixelTransferf(GL_GREEN_BIAS, 0.0f);
	glPixelTransferf(GL_BLUE_BIAS, 0.0f);
}

////////////////////////////////////////////////////////////
/// Properties
////////////////////////////////////////////////////////////
void Sprite::setViewport(VALUE nviewport)
{
	if (viewport_ != nviewport) {
		if ( ! NIL_P(nviewport) ) {
			Graphics::RemoveZObj(id);
			Viewport::get(nviewport).RegisterZObj(0, id);
		} else {
			if ( ! NIL_P(viewport_) ) Viewport::get(viewport_).RemoveZObj(id);
			Graphics::RegisterZObj(0, id);
		}
	}
	viewport_ = nviewport;
}
void Sprite::setBitmap(VALUE nbitmap)
{
	if (bitmap_ != nbitmap) {
		needs_refresh = true;
		flash_needs_refresh = true;
	}
	bitmap_ = nbitmap;
}
void Sprite::setZ(int nz)
{
	if (z != nz) {
		if ( ! NIL_P(viewport_) ) Viewport::get(viewport_).UpdateZObj(id, nz);
		else Graphics::UpdateZObj(id, nz);
	}
	z = nz;
}
void Sprite::setTone(VALUE ntone)
{
	if (tone != ntone) needs_refresh = true;
	tone = ntone;
}
