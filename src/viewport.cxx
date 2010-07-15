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
#include "viewport.hxx"
#include "sprite.hxx"
#include "plane.hxx"
#include "tilemap_xp.hxx"
#include "window_xp.hxx"
#include <argss/ruby.hxx>
#include "graphics.hxx"
#include "player.hxx"

////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////
Viewport::Viewport(VALUE iid)
: id_(iid)
, rect_(rb_iv_get(id_, "@rect"))
, visible_(true)
, z_(0)
, ox_(0)
, oy_(0)
, color_(rb_iv_get(id_, "@color"))
, tone_(rb_iv_get(id_, "@tone"))
, flash_duration_(0)
, disposing_(false)
, dstRect_(rect_)
{
	Graphics::RegisterZObj(0, id_);
}

////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Viewport::~Viewport()
{
}

////////////////////////////////////////////////////////////
/// Class Is Viewport Disposed?
////////////////////////////////////////////////////////////
bool Viewport::IsDisposed(VALUE id)
{
	return Graphics::countDrawable(id) == 0;
}

////////////////////////////////////////////////////////////
/// Class New Viewport
////////////////////////////////////////////////////////////
void Viewport::New(VALUE id)
{
	Graphics::insertDrawable( id, boost::shared_ptr< Drawable >( new Viewport(id) ) );
}

////////////////////////////////////////////////////////////
/// Class get Viewport
////////////////////////////////////////////////////////////
Viewport& Viewport::get(VALUE id)
{
	return dynamic_cast< Viewport& >( Graphics::getDrawable(id) );
}

////////////////////////////////////////////////////////////
/// Class Dispose Viewport
////////////////////////////////////////////////////////////
void Viewport::Dispose(VALUE id)
{
	Graphics::eraseDrawable(id);
	Graphics::RemoveZObj(id);
}

////////////////////////////////////////////////////////////
/// Refresh Bitmaps
////////////////////////////////////////////////////////////
void Viewport::RefreshBitmaps()
{
}

////////////////////////////////////////////////////////////
/// Draw
////////////////////////////////////////////////////////////
void Viewport::draw(long z)
{
	if (!visible_) return;

	dstRect_ = Rect(rect_);

	if (dstRect_.x < -dstRect_.width || dstRect_.x > Player::getWidth() || dstRect_.y < -dstRect_.height || dstRect_.y > Player::getHeight()) return;

	for(std::list< ZObj >::const_iterator it = zlist_.begin(); it != zlist_.end(); it++) {
		Graphics::getDrawable( it->getId() ).draw(it->getZ());
	}
}

////////////////////////////////////////////////////////////
/// Update
////////////////////////////////////////////////////////////
void Viewport::Update()
{
	if (flash_duration_ != 0) {
		flash_frame_ += 1;
		if (flash_duration_ == flash_frame_) {
			flash_duration_ = 0;
		}
	}
}

////////////////////////////////////////////////////////////
/// Flash
////////////////////////////////////////////////////////////
void Viewport::Flash(int duration)
{
	flash_color_ = Color(0, 0, 0, 0);
	flash_duration_ = duration;
	flash_frame_ = 0;
}
void Viewport::Flash(Color const& color, int duration)
{
	flash_color_ = color;
	flash_duration_ = duration;
	flash_frame_ = 0;
}

////////////////////////////////////////////////////////////
/// Properties
////////////////////////////////////////////////////////////
void Viewport::setZ(int nz) {
	if (z_ != nz) Graphics::UpdateZObj(id_, nz);
	z_ = nz;
}

////////////////////////////////////////////////////////////
/// Register ZObj
////////////////////////////////////////////////////////////
void Viewport::RegisterZObj(long z, VALUE id)
{
	Graphics::incrementCreation();
	ZObj zobj(z, Graphics::getCreation(), id_);
	zlist_.push_back(zobj);
	zlist_.sort(Graphics::SortZObj);
}
void Viewport::RegisterZObj(long z, VALUE id, bool multiz)
{
	ZObj zobj(z, 999999, id_);
	zlist_.push_back(zobj);
	zlist_.sort(Graphics::SortZObj);
}

////////////////////////////////////////////////////////////
/// Remove ZObj
////////////////////////////////////////////////////////////
struct remove_zobj_id : public std::binary_function< ZObj, ZObj, bool >
{
	remove_zobj_id(VALUE val) : id(val) {}
	bool operator () (ZObj &obj) const {return obj.getId() == id;}
	VALUE id;
};
void Viewport::RemoveZObj(VALUE id)
{
	if (disposing_) return;
	zlist_.remove_if(remove_zobj_id(id));
}

////////////////////////////////////////////////////////////
/// Update ZObj Z
////////////////////////////////////////////////////////////
void Viewport::UpdateZObj(VALUE id, long z)
{
	for(std::list< ZObj >::iterator it = zlist_.begin(); it != zlist_.end(); it++) {
		if (it->getId() == id) {
			it->setZ(z);
			break;
		}
	}
	zlist_.sort(Graphics::SortZObj);
}
