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

#ifndef _GRAPHICS_HXX_
#define _GRAPHICS_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <boost/smart_ptr.hpp>

#include <string>
#include <list>

#include "argss_ruby.hxx"

#include "color.hxx"
#include "drawable.hxx"
#include "zobj.hxx"

////////////////////////////////////////////////////////////
/// Graphics namespace
////////////////////////////////////////////////////////////
namespace Graphics
{
	void Init();
	void InitOpenGL();
	void Exit();
	void RefreshAll();
	void TimerWait();
	void TimerContinue();
	void DrawFrame();

	void Update();
	void Freeze();
	void Transition(int duration, std::string const& filename, int vague);
	void FrameReset();
	void Wait(int duration);
	void ResizeScreen(int width, int height);
	VALUE SnapToBitmap();
	void FadeOut(int duration);
	void FadeIn(int duration);
	int getFrameRate();
	void setFrameRate(int nframerate);
	int getFrameCount();
	void setFrameCount(int nframecount);
	VALUE getBackColor();
	void setBackColor(VALUE nbackcolor);
	int getBrightness();
	void setBrightness(int nbrightness);

	bool SortZObj(ZObj &first, ZObj &second);
	void RegisterZObj(long z, VALUE id);
	void RegisterZObj(long z, VALUE id, bool multiz);
	void RemoveZObj(VALUE id);
	void UpdateZObj(VALUE id, long z);

	Drawable& getDrawable(VALUE id);
	unsigned int countDrawable(VALUE id);
	bool insertDrawable(VALUE id, boost::shared_ptr< Drawable > const& ptr);
	void eraseDrawable(VALUE id);

	int getFPS();
	void incrementCreation();
	long getCreation();
} // namespace Graphics

#endif
