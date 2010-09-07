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
#include <boost/format.hpp>
#include <boost/unordered_map.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <GL/gl.h>

#include <argss/error.hxx>
#include <argss/bitmap.hxx>
#include <argss/ruby.hxx>

#include "time.hxx"
#include "graphics.hxx"
#include "system.hxx"
#include "player.hxx"
#include "options.hxx"
#include "output.hxx"
#include "sprite.hxx"
#include "tilemap.hxx"
#include "text.hxx"


namespace Graphics
{
	namespace
	{
		////////////////////////////////////////////////////////////
		/// Global Variables
		////////////////////////////////////////////////////////////
		int fps;
		int frameRate_;
		int frameCount_;
		Color backcolor; // (255.0f, 255.0f, 255.0f, 255.0f);
		int brightness_;
		double frameInterval_;
		boost::unordered_map< VALUE, boost::shared_ptr< Drawable > > drawableMap_;
		std::list< ZObj > zlist_;
		long creation;
		long lastTics_;
		long lastTicsWait_;
		long nextTicsFps_;
		VALUE freezing_ = Qnil; // "NIL_P(freezing_) == false" if freezing the screen

		bool isFreezing() { return !NIL_P(freezing_); }

		int const SCREEN_WIDTH_MAX = 640, SCREEN_HEIGHT_MAX = 480;
	}

	int getWidth () { return Player::getMainWindow().getWidth (); }
	int getHeight() { return Player::getMainWindow().getHeight(); }

	int getFPS() { return fps; }
	Drawable& getDrawable(VALUE id)
	{
		assert( drawableMap_.find(id) != drawableMap_.end() );
		return *( drawableMap_.find(id)->second );
	}
	bool insertDrawable(VALUE id, boost::shared_ptr< Drawable > const& ptr)
	{
		return drawableMap_.insert( std::make_pair(id, ptr) ).second;
	}
	void eraseDrawable(VALUE id)
	{
		assert( drawableMap_.find(id) != drawableMap_.end() );
		drawableMap_.erase( drawableMap_.find(id) );
	}
	unsigned int countDrawable(VALUE id)
	{
		return drawableMap_.count(id);
	}
	void incrementCreation() { creation++; }
	long getCreation() { return creation; }

	////////////////////////////////////////////////////////////
	/// Initialize
	////////////////////////////////////////////////////////////
	void Init()
	{
		fps = 0;
		setFrameRate(DEFAULT_FPS);
		frameCount_ = 0;
		backcolor = Color(0, 0, 0, 0);
		brightness_ = 255;
		creation = 0;
		lastTics_ = Time::getTime() + long(frameInterval_);
		nextTicsFps_ = Time::getTime() + 1000;

		InitOpenGL();

		Text::Init();
		Tilemap::Init();
	}

	////////////////////////////////////////////////////////////
	/// Initialize OpengGL
	////////////////////////////////////////////////////////////
	void InitOpenGL()
	{
		glViewport(0, 0, Player::getWidth(), Player::getHeight());
		glShadeModel(GL_FLAT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, Player::getWidth(), Player::getHeight(), 0, -1, 1); 

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_BLEND);

		glClearColor((GLclampf)(backcolor.red   / 255.0f),
					 (GLclampf)(backcolor.green / 255.0f),
					 (GLclampf)(backcolor.blue  / 255.0f),
					 (GLclampf)(backcolor.alpha / 255.0f));
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		Player::swapBuffers();
	}

	////////////////////////////////////////////////////////////
	/// Exit
	////////////////////////////////////////////////////////////
	void Exit()
	{
		drawableMap_.clear();
		Bitmap::DisposeBitmaps();
	}

	////////////////////////////////////////////////////////////
	/// Refresh all graphic objects
	////////////////////////////////////////////////////////////
	void RefreshAll()
	{
		boost::unordered_map< VALUE, boost::shared_ptr< Drawable > >::iterator it;
		for (it = drawableMap_.begin(); it != drawableMap_.end(); ++it) {
			it->second->RefreshBitmaps();
		}
		Bitmap::RefreshBitmaps();
	}

	////////////////////////////////////////////////////////////
	/// Wait
	////////////////////////////////////////////////////////////
	void TimerWait()
	{
		lastTicsWait_ = Time::getTime();
	}

	////////////////////////////////////////////////////////////
	/// Continue
	////////////////////////////////////////////////////////////
	void TimerContinue()
	{
		lastTics_ += Time::getTime() - lastTicsWait_;
		nextTicsFps_ += Time::getTime() - lastTicsWait_;
	}

	////////////////////////////////////////////////////////////
	/// Update
	////////////////////////////////////////////////////////////
	void Update()
	{
		// static long tics;
		// static long ticsFps = Time::getTime();
		static long frames = 0;
		// static double waitFrames = 0;
		// static double cyclesLeftOver;

		Player::Update();
//		if (waitFrames >= 1) {
//			waitFrames -= 1;
//			return;
//		}
		int tics = Time::getTime();

		if ((tics - lastTics_) >= frameInterval_) {// || (frameInterval_ - tics + lastTics_) < 12) {
			//cyclesLeftOver = waitFrames;
			//waitFrames = (double)(tics - lastTics_) / frameInterval_ - cyclesleftover;
			//lastTics_ += (tics - lastTics_);
			//lastTics_ = tics;

			drawFrame();

			// run GC if time is left
			if( (Time::getTime() - tics) < frameInterval_ ) rb_gc();

			frameCount_++;
			frames++;

			if (tics >= nextTicsFps_) {
				fps = frames;
				frames = 0;

				FrameReset();

				Player::getMainWindow().setTitle( ( boost::format("%s - %d FPS") % System::getTitle() % fps ).str() );
			}
		} else {
			Time::sleepMs(frameInterval_ - (tics - lastTics_));
		}
	}

	////////////////////////////////////////////////////////////
	/// Draw Frame
	////////////////////////////////////////////////////////////
	void drawFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		if( isFreezing() ) {
			GLshort vertexes[4][2];
			vertexes[0][0] = 0; vertexes[0][1] = 0;
			vertexes[1][0] = 0; vertexes[1][1] = Player::getHeight();
			vertexes[2][0] = Player::getWidth(); vertexes[2][1] = Player::getHeight();
			vertexes[3][0] = Player::getWidth(); vertexes[3][1] = 0;
			GLfloat coords[4][2] = { {0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}, };
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnable(GL_TEXTURE_2D);
				Bitmap::get(freezing_).BindBitmap();
				glVertexPointer(2, GL_FLOAT, 0, vertexes);
				glTexCoordPointer(2, GL_FLOAT, 0, coords);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		} else {
			for (std::list< ZObj >::iterator it = zlist_.begin(); it != zlist_.end(); ++it) {
				getDrawable( it->getId() ).draw(it->getZ());
			}
		}

		if (brightness_ < 255) {
			glDisable(GL_TEXTURE_2D);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glColor4f(0.0f, 0.0f, 0.0f, (float)(1.0f - brightness_ / 255.0f));

			GLshort vertexes[4][2];
			vertexes[0][0] = 0; vertexes[0][1] = 0;
			vertexes[1][0] = 0; vertexes[1][1] = Player::getHeight();
			vertexes[2][0] = Player::getWidth(); vertexes[2][1] = Player::getHeight();
			vertexes[3][0] = Player::getWidth(); vertexes[3][1] = 0;

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_SHORT, 0, vertexes);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glDisableClientState(GL_VERTEX_ARRAY);
		}

		Player::swapBuffers();
	}

	////////////////////////////////////////////////////////////
	/// Freeze screen
	////////////////////////////////////////////////////////////
	void Freeze()
	{
		if( isFreezing() ) ARGSS::ABitmap::rdispose(freezing_);
		freezing_ = SnapToBitmap();
	}

	////////////////////////////////////////////////////////////
	/// Transition effect
	////////////////////////////////////////////////////////////
	void Transition(int duration, std::string const& filename, int vague)
	{
		if( isFreezing() ) {
			ARGSS::ABitmap::rdispose(freezing_);
			freezing_ = Qnil;
		}

		// TODO
	}

	////////////////////////////////////////////////////////////
	/// Reset frames
	////////////////////////////////////////////////////////////
	void FrameReset()
	{
		lastTics_ = Time::getTime();
		nextTicsFps_ = Time::getTime() + 1000;
	}

	////////////////////////////////////////////////////////////
	/// Wait frames
	////////////////////////////////////////////////////////////
	void Wait(int duration)
	{
		for(int i = duration; i > 0; i--) {
			Update();
		}
	}

	////////////////////////////////////////////////////////////
	/// Resize screen
	////////////////////////////////////////////////////////////
	void ResizeScreen(int width, int height)
	{
		// clamp
		if(width  > SCREEN_WIDTH_MAX ) width  = SCREEN_WIDTH_MAX ;
		if(height > SCREEN_HEIGHT_MAX) height = SCREEN_HEIGHT_MAX;

		Player::ResizeWindow(width, height);

		glViewport(0, 0, width, height);
		glShadeModel(GL_FLAT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, 1); 

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);

		Player::swapBuffers();
	}

	////////////////////////////////////////////////////////////
	/// Snap scree to bitmap
	////////////////////////////////////////////////////////////
	VALUE SnapToBitmap() // RGSS2
	{
		VALUE argv[] = { INT2NUM( getWidth () ), INT2NUM( getHeight() ), };
		VALUE ret = rb_class_new_instance(2, argv, ARGSS::ABitmap::getID() );

		if( isFreezing() ) {
			// TODO
		} else {
			Uint32* dst = Bitmap::get(ret).getPixels();
			std::vector< Uint32 > src( getWidth() * getHeight() );
			std::size_t cpySize = sizeof(Uint32) * getHeight();

			// glReadPixels() isn't available in GL ES
			glReadPixels( 0, 0, getWidth(), getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, &(src[0]) );
			for(int y = 0; y < getHeight(); y++) {
				std::memcpy( &(dst[getWidth() * y]), &(src[getWidth() * (getHeight() - y - 1)]), cpySize );
			}
		}

		return ret;
	}

	////////////////////////////////////////////////////////////
	/// Fade out
	////////////////////////////////////////////////////////////
	void FadeOut(int duration) // RGSS2
	{
		int n = brightness_ / duration;
		for (;duration > 0; duration--) {
			brightness_ -= n;
			Update();
		}
		if (brightness_ > 0) {
			brightness_ = 0;
			Update();
		}
	}


	////////////////////////////////////////////////////////////
	/// Fade in
	////////////////////////////////////////////////////////////
	void FadeIn(int duration) // RGSS2
	{
		int n = 255 / duration;
		for (;duration > 0; duration--) {
			brightness_ += n;
			Update();
		}
		if (brightness_ < 255) {
			brightness_ = 255;
			Update();
		}
	}

	////////////////////////////////////////////////////////////
	/// Properties
	////////////////////////////////////////////////////////////
	int getFrameRate()
	{
		return frameRate_;
	}
	void setFrameRate(int nframerate)
	{
		frameRate_ = nframerate;
		frameInterval_ = 1000.0 / frameRate_;
	}
	int getFrameCount()
	{
		return frameCount_;
	}
	void setFrameCount(int nframecount)
	{
		frameCount_ = nframecount;
	}
	VALUE getBackColor()
	{
		return backcolor.getARGSS();
	}
	void setBackColor(VALUE nbackcolor)
	{
		backcolor = Color(nbackcolor);
		glClearColor((GLclampf)(backcolor.red / 255.0f),
					 (GLclampf)(backcolor.green / 255.0f),
					 (GLclampf)(backcolor.blue / 255.0f),
					 (GLclampf)(backcolor.alpha / 255.0f));
	}
	int getBrightness() // RGSS2
	{
		return brightness_;
	}
	void setBrightness(int nbrightness) // RGSS2
	{
		brightness_ = nbrightness;
	}

	////////////////////////////////////////////////////////////
	/// Sort ZObj
	////////////////////////////////////////////////////////////
	bool SortZObj(ZObj &first, ZObj &second)
	{
		if (first.getZ() < second.getZ()) return true;
		else if (first.getZ() > second.getZ()) return false;
		else return first.getCreation() < second.getCreation();
	}

	////////////////////////////////////////////////////////////
	/// Register ZObj
	////////////////////////////////////////////////////////////
	void RegisterZObj(long z, VALUE id)
	{
		zlist_.push_back( ZObj(z, ++creation, id) );
		zlist_.sort(SortZObj);
	}
	void RegisterZObj(long z, VALUE id, bool multiz)
	{
		zlist_.push_back( ZObj(z, 999999, id) );
		zlist_.sort(SortZObj);
	}

	////////////////////////////////////////////////////////////
	/// Remove ZObj
	////////////////////////////////////////////////////////////
	struct remove_zobj_id : public std::binary_function<ZObj, ZObj, bool>
	{
		remove_zobj_id(VALUE val) : id(val) {}
		bool operator () (ZObj &obj) const {return obj.getId() == id;}
		VALUE id;
	};
	void RemoveZObj(VALUE id)
	{
		zlist_.remove_if( remove_zobj_id(id) );
	}

	////////////////////////////////////////////////////////////
	/// Update ZObj Z
	////////////////////////////////////////////////////////////
	void UpdateZObj(VALUE id, long z)
	{
		for(std::list< ZObj >::iterator it = zlist_.begin(); it != zlist_.end(); ++it) {
			if (it->getId() == id) {
				it->setZ(z);
				break;
			}
		}
		zlist_.sort(SortZObj);
	}
} // namespace Graphics
