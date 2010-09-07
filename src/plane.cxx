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
#include <cmath>
#include <cstring>

#include <iostream>
#include <string>
#include <vector>

#include <boost/make_shared.hpp>

#include "plane.hxx"
#include <argss/ruby.hxx>
#include <argss/plane.hxx>
#include "graphics.hxx"
#include "viewport.hxx"
#include "rect.hxx"
#include "player.hxx"

////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////
Plane::Plane(VALUE iid)
: id(iid), viewport_( rb_iv_get(id, "@viewport") ), bitmap_(Qnil)
, visible_(true), z(0), ox(0),  oy(0), zoom_x(1.0), zoom_y(1.0)
, opacity(255), blendType_(Blend::NORMAL)
, color( rb_iv_get(id, "@color") ), tone( rb_iv_get(id, "@tone") )
{
	if ( ! NIL_P(viewport_) ) Viewport::get(viewport_).RegisterZObj(0, id);
	else Graphics::RegisterZObj(0, id);
}

////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Plane::~Plane()
{
}

////////////////////////////////////////////////////////////
/// Class Is Plane Disposed?
////////////////////////////////////////////////////////////
bool Plane::IsDisposed(VALUE id)
{
	return Graphics::countDrawable(id) == 0;
}

////////////////////////////////////////////////////////////
/// Class New Plane
////////////////////////////////////////////////////////////
void Plane::New(VALUE id)
{
	Graphics::insertDrawable( id, boost::make_shared<Plane>(id) );
}

////////////////////////////////////////////////////////////
/// Class get Plane
////////////////////////////////////////////////////////////
Plane& Plane::get(VALUE id)
{
	return dynamic_cast< Plane& >( Graphics::getDrawable(id) );
}

////////////////////////////////////////////////////////////
/// Class Dispose Plane
////////////////////////////////////////////////////////////
void Plane::Dispose(VALUE id)
{
	if ( ! NIL_P(Plane::get(id).viewport_) ) Viewport::get(Plane::get(id).viewport_).RemoveZObj(id);
	else Graphics::RemoveZObj(id);

	Graphics::eraseDrawable(id);
}

////////////////////////////////////////////////////////////
/// Refresh Bitmaps
////////////////////////////////////////////////////////////
void Plane::RefreshBitmaps()
{
}

////////////////////////////////////////////////////////////
/// Draw
////////////////////////////////////////////////////////////
void Plane::draw(long z)
{
	if( (!visible_) || NIL_P(bitmap_) ) return;

	Bitmap& bmp = Bitmap::get(bitmap_);
	bmp.Refresh();

	glEnable(GL_TEXTURE_2D);

	bmp.BindBitmap();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float rectw, recth;

	if ( ! NIL_P(viewport_) ) {
		Rect const& rect = Viewport::get(viewport_).getViewportRect();

		glEnable(GL_SCISSOR_TEST);
		glScissor(rect.x, Player::getHeight() - (rect.y + rect.height), rect.width, rect.height);

		rectw = (float)rect.width;
		recth = (float)rect.height;

		glTranslatef((float)rect.x, (float)rect.y, 0.0f);
	} else {
		rectw = (float)Player::getWidth();
		recth = (float)Player::getHeight();
	}

	glColor4f(1.0f, 1.0f, 1.0f, opacity / 255.0f);

	glEnable(GL_BLEND);
	setBlendFunc(blendType_);

	float bmpw = bmp.getWidth() * zoom_x;
	float bmph = bmp.getHeight() * zoom_y;
	int r_ox = -ox % (int)bmpw;
	int r_oy = -oy % (int)bmph;
	float tilesx = std::ceil(rectw / bmpw) + std::ceil(r_ox / bmpw);
	float tilesy = std::ceil(recth / bmph) + std::ceil(r_oy / bmph);
	while (r_ox > 0) r_ox -= (int)bmpw;
	while (r_oy > 0) r_oy -= (int)bmph;
	GLfloat coords[4][2] = { {0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}, };
	GLfloat vertexes[4][2];
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	for (float j = r_oy / bmph; j < tilesy; j++) {
	for (float i = r_ox / bmpw; i < tilesx; i++) {
		vertexes[0][0] = i * bmpw; vertexes[0][1] = j * bmph;
		vertexes[1][0] = (i + 1) * bmpw; vertexes[1][1] = j * bmph;
		vertexes[2][0] = (i + 1) * bmpw; vertexes[2][1] = (j + 1) * bmph;
		vertexes[3][0] = i * bmpw; vertexes[3][1] = (j + 1) * bmph;

		glVertexPointer(2, GL_FLOAT, 0, vertexes);
		glTexCoordPointer(2, GL_FLOAT, 0, coords);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_SCISSOR_TEST);
}
void Plane::draw(long z, Bitmap const& dst_bitmap)
{
}

////////////////////////////////////////////////////////////
/// Properties
////////////////////////////////////////////////////////////
void Plane::setViewport(VALUE nviewport)
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
void Plane::setBitmap(VALUE nbitmap)
{
	if (bitmap_ != nbitmap) needs_refresh = true;
	bitmap_ = nbitmap;
}
void Plane::setZ(int nz)
{
	if (z != nz) {
		if ( !NIL_P(viewport_) ) Viewport::get(viewport_).UpdateZObj(id, nz);
		else Graphics::UpdateZObj(id, nz);
	}
	z = nz;
}
void Plane::setZoomX(float nzoom_x)
{
	if (zoom_x != nzoom_x) needs_refresh = true;
	zoom_x = nzoom_x;
}
void Plane::setZoomY(float nzoom_y)
{
	if (zoom_y != nzoom_y) needs_refresh = true;
	zoom_y = nzoom_y;
}
void Plane::setOpacity(int nopacity)
{
	if (opacity != nopacity) needs_refresh = true;
	opacity = nopacity;
}
void Plane::setTone(VALUE ntone)
{
	if (tone != ntone) needs_refresh = true;
	tone = ntone;
}
