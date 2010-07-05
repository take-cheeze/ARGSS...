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

#include <iostream>
#include <map>
#include <string>

#include <GL/gl.h>

#include "time.hxx"
#include "graphics.hxx"
#include "argss_ruby.hxx"
#include "argss_error.hxx"
#include "system.hxx"
#include "player.hxx"
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
		int framerate;
		int framecount;
		Color backcolor;
		int brightness;
		double framerate_interval;
		std::map< VALUE, boost::shared_ptr< Drawable > > drawableMap_;
		std::map< VALUE, boost::shared_ptr< Drawable > >::iterator it_drawable_map;
		std::list<ZObj> zlist;
		std::list<ZObj>::iterator it_zlist;
		long creation;
		long last_tics;
		long last_tics_wait;
		long next_tics_fps;
	}

	int getFPS() { return fps; }
	Drawable& getDrawable(VALUE id)
	{
		assert( drawableMap_.find(id) != drawableMap_.end() );
		return *( drawableMap_.find(id)->second );
	}
	bool insertDrawable(VALUE id, boost::shared_ptr< Drawable > const& ptr)
	{
		return drawableMap_.insert(
			std::map< VALUE, boost::shared_ptr< Drawable > >::value_type(id, ptr)
		).second;
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
		framerate = 40;
		framecount = 0;
		backcolor = Color(0, 0, 0, 0);
		brightness = 255;
		creation = 0;
		framerate_interval = 1000.0 / framerate;
		last_tics = Time::getTime() + (long)framerate_interval;
		next_tics_fps = Time::getTime() + 1000;

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
		for (it_drawable_map = drawableMap_.begin(); it_drawable_map != drawableMap_.end(); it_drawable_map++) {
			it_drawable_map->second->RefreshBitmaps();
		}
		Bitmap::RefreshBitmaps();
	}

	////////////////////////////////////////////////////////////
	/// Wait
	////////////////////////////////////////////////////////////
	void TimerWait()
	{
		last_tics_wait = Time::getTime();
	}

	////////////////////////////////////////////////////////////
	/// Continue
	////////////////////////////////////////////////////////////
	void TimerContinue()
	{
		last_tics += Time::getTime() - last_tics_wait;
		next_tics_fps += Time::getTime() - last_tics_wait;
	}

	////////////////////////////////////////////////////////////
	/// Update
	////////////////////////////////////////////////////////////
	void Update()
	{
		// Player::getMainWindow().makeCurrent();

		static long tics;
		// static long tics_fps = Time::getTime();
		static long frames = 0;
		// static double waitframes = 0;
		// static double cyclesleftover;

		Player::Update();
		/*if (waitframes >= 1) {
			waitframes -= 1;
			return;
		}*/
		tics = Time::getTime();

		// std::cout << tics << std::endl;

		if ((tics - last_tics) >= framerate_interval) {// || (framerate_interval - tics + last_tics) < 12) {
			//cyclesleftover = waitframes;
			//waitframes = (double)(tics - last_tics) / framerate_interval - cyclesleftover;
			//last_tics += (tics - last_tics);
			last_tics = tics;

			DrawFrame();

			framecount++;
			frames++;

			if (tics >= next_tics_fps) {
				next_tics_fps += 1000;
				fps = frames;
				frames = 0;

				Player::getMainWindow().setTitle( ( boost::format("%s - %d FPS") % System::getTitle() % fps ).str() );
			}
		}
		else {
			Time::SleepMs((long)(framerate_interval) - (tics - last_tics));
		}
	}

	////////////////////////////////////////////////////////////
	/// Draw Frame
	////////////////////////////////////////////////////////////
	void DrawFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		for (it_zlist = zlist.begin(); it_zlist != zlist.end(); it_zlist++) {
			drawableMap_[it_zlist->getId()]->Draw(it_zlist->getZ());
		}

		if (brightness < 255) {
			glDisable(GL_TEXTURE_2D);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glColor4f(0.0f, 0.0f, 0.0f, (float)(1.0f - brightness / 255.0f));
			glBegin(GL_QUADS);
				glVertex2i(0, 0);
				glVertex2i(0, Player::getHeight());
				glVertex2i(Player::getWidth(), Player::getHeight());
				glVertex2i(Player::getWidth(), 0);
			glEnd();
		}

		Player::swapBuffers();
	}

	////////////////////////////////////////////////////////////
	/// Freeze screen
	////////////////////////////////////////////////////////////
	void Freeze()
	{
		// TODO
	}

	////////////////////////////////////////////////////////////
	/// Transition effect
	////////////////////////////////////////////////////////////
	void Transition(int duration, std::string const& filename, int vague)
	{
		// TODO
	}

	////////////////////////////////////////////////////////////
	/// Reset frames
	////////////////////////////////////////////////////////////
	void FrameReset()
	{
		last_tics = Time::getTime();
		next_tics_fps = Time::getTime() + 1000;
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
	VALUE SnapToBitmap()
	{
		return Qnil;
	}

	////////////////////////////////////////////////////////////
	/// Fade out
	////////////////////////////////////////////////////////////
	void FadeOut(int duration)
	{
		int n = brightness / duration;
		for (;duration > 0; duration--) {
			brightness -= n;
			Update();
		}
		if (brightness > 0) {
			brightness = 0;
			Update();
		}
	}


	////////////////////////////////////////////////////////////
	/// Fade in
	////////////////////////////////////////////////////////////
	void FadeIn(int duration)
	{
		int n = 255 / duration;
		for (;duration > 0; duration--) {
			brightness += n;
			Update();
		}
		if (brightness < 255) {
			brightness = 255;
			Update();
		}
	}

	////////////////////////////////////////////////////////////
	/// Properties
	////////////////////////////////////////////////////////////
	int getFrameRate()
	{
		return framerate;
	}
	void setFrameRate(int nframerate)
	{
		framerate = nframerate;
		framerate_interval = 1000.0 / framerate;
	}
	int getFrameCount()
	{
		return framecount;
	}
	void setFrameCount(int nframecount)
	{
		framecount = nframecount;
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
	int getBrightness()
	{
		return brightness;
	}
	void setBrightness(int nbrightness)
	{
		brightness = nbrightness;
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
		creation += 1;
		ZObj zobj(z, creation, id);

		zlist.push_back(zobj);
		zlist.sort(SortZObj);
	}
	void RegisterZObj(long z, VALUE id, bool multiz)
	{
		ZObj zobj(z, 999999, id);
		zlist.push_back(zobj);
		zlist.sort(SortZObj);
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
		zlist.remove_if (remove_zobj_id(id));
	}

	////////////////////////////////////////////////////////////
	/// Update ZObj Z
	////////////////////////////////////////////////////////////
	void UpdateZObj(VALUE id, long z)
	{
		for(it_zlist = zlist.begin(); it_zlist != zlist.end(); it_zlist++) {
			if (it_zlist->getId() == id) {
				it_zlist->setZ(z);
				break;
			}
		}
		zlist.sort(SortZObj);
	}
} // namespace Graphics
