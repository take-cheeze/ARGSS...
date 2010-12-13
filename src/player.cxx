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
#include <cassert>
#include <memory>

#include "argss/argss.hxx"

#include "audio.hxx"
#include "event.hxx"
#include "graphics.hxx"
#include "input.hxx"
#include "options.hxx"
#include "output.hxx"
#include "player.hxx"
#include "system.hxx"
#include "windowui.hxx"


namespace Player
{
	namespace
	{
		////////////////////////////////////////////////////////////
		/// Global Variables
		////////////////////////////////////////////////////////////
		std::auto_ptr<WindowUi> mainWindow_;
		bool focus_;
		bool altPressing_;
	} // namespace

	WindowUi& getMainWindow()
	{
		assert( mainWindow_.get() );
		return *mainWindow_;
	}

	////////////////////////////////////////////////////////////
	/// Initialize
	////////////////////////////////////////////////////////////
	void Init()
	{
		mainWindow_.reset(
			new WindowUi(System::getDefaultWidth(), System::getDefaultHeight(), System::getTitle(), true, RUN_FULLSCREEN)
		);

		focus_ = true;
		altPressing_ = false;
	}

	////////////////////////////////////////////////////////////
	/// Initialize
	////////////////////////////////////////////////////////////
	void Update()
	{
		while (true) {
			boost::optional<Event> evnt = getMainWindow().popEvent();
			if(!evnt) break;

			switch(evnt->type) {
				case Event::Quit:
					ARGSS::Exit();
					return;
				case Event::KeyDown:
					if (evnt->param1 == Input::Keys::ALT) {
						altPressing_ = true;
					}
					else if (evnt->param1 == Input::Keys::RETURN && altPressing_) {
						ToggleFullscreen();
						altPressing_ = false;
					}
					break;
				case Event::KeyUp:
					if (evnt->param1 == Input::Keys::ALT) {
						altPressing_ = false;
					}
					break;
				default: if(PAUSE_GAME_WHEN_FOCUS_LOST) {
					if (evnt->type == Event::GainFocus && !focus_) {
						focus_ = true;
						Graphics::TimerContinue();
						if (PAUSE_AUDIO_WHEN_FOCUS_LOST) {
							Audio::Continue();
						}
					} else if (evnt->type == Event::LostFocus && focus_) {
						focus_ = false;
						Input::ClearKeys();
						Graphics::TimerWait();
						if (PAUSE_AUDIO_WHEN_FOCUS_LOST) {
							Audio::Pause();
						}
					}
				} break;
			}
		}
	}

	////////////////////////////////////////////////////////////
	/// Exit
	////////////////////////////////////////////////////////////
	void Exit()
	{
		Graphics::Exit();
		Output::None();
		mainWindow_.release();
	}

	////////////////////////////////////////////////////////////
	/// Switch fullscreen
	////////////////////////////////////////////////////////////
	void ToggleFullscreen()
	{
		bool toggle = !getMainWindow().isFullscreen();
		mainWindow_.reset(
			new WindowUi(System::getDefaultWidth(), System::getDefaultHeight(), System::getTitle(), true, toggle)
		);
		Graphics::InitOpenGL();
		Graphics::RefreshAll();
	}

	////////////////////////////////////////////////////////////
	/// Resize window
	////////////////////////////////////////////////////////////
	void ResizeWindow(long width, long height)
	{
		getMainWindow().resize(width, height);
	}

	////////////////////////////////////////////////////////////
	/// get window width
	////////////////////////////////////////////////////////////
	int getWidth()
	{
		return getMainWindow().getWidth();
	}

	////////////////////////////////////////////////////////////
	/// get window height
	////////////////////////////////////////////////////////////
	int getHeight()
	{
		return getMainWindow().getHeight();
	}

	////////////////////////////////////////////////////////////
	/// Swap Buffers
	////////////////////////////////////////////////////////////
	void swapBuffers()
	{
		getMainWindow().swapBuffers();
	}
} // namespace Player
