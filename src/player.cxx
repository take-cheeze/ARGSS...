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

#include "player.hxx"
#include "options.hxx"
#include "system.hxx"
#include "output.hxx"
#include "input.hxx"
#include "graphics.hxx"
#include "audio.hxx"
#include "argss.hxx"

namespace Player
{
	namespace
	{
		////////////////////////////////////////////////////////////
		/// Global Variables
		////////////////////////////////////////////////////////////
		WindowUi* main_window;
		bool focus;
		bool alt_pressing;
	} // namespace

	WindowUi& getMainWindow()
	{
		assert(main_window);
		return *main_window;
	}

	////////////////////////////////////////////////////////////
	/// Initialize
	////////////////////////////////////////////////////////////
	void Init()
	{
		main_window = new WindowUi(System::Width, System::Height, System::Title, true, RUN_FULLSCREEN);

		focus = true;
		alt_pressing = false;
	}

	////////////////////////////////////////////////////////////
	/// Initialize
	////////////////////////////////////////////////////////////
	void Update()
	{
		Event evnt;

		while (true) {
			bool result = getMainWindow().GetEvent(evnt);
			if (evnt.type == Event::Quit) {
				ARGSS::Exit();
				break;
			}
			else if (evnt.type == Event::KeyDown) {
				if (evnt.param1 == Input::Keys::ALT) {
					alt_pressing = true;
				}
				else if (evnt.param1 == Input::Keys::RETURN && alt_pressing) {
					ToggleFullscreen();
					alt_pressing = false;
				}
			}
			else if (evnt.type == Event::KeyUp) {
				if (evnt.param1 == Input::Keys::ALT) {
					alt_pressing = false;
				}
			}
			else if (PAUSE_GAME_WHEN_FOCUS_LOST) {
				if (evnt.type == Event::GainFocus && !focus) {
					focus = true;
					Graphics::TimerContinue();
					if (PAUSE_AUDIO_WHEN_FOCUS_LOST) {
						Audio::Continue();
					}
				}
				else if (evnt.type == Event::LostFocus && focus) {
					focus = false;
					Input::ClearKeys();
					Graphics::TimerWait();
					if (PAUSE_AUDIO_WHEN_FOCUS_LOST) {
						Audio::Pause();
					}
				}
			}

			if (!result && !(PAUSE_GAME_WHEN_FOCUS_LOST && !focus)) {
				break;
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
		getMainWindow().Dispose();
	}

	////////////////////////////////////////////////////////////
	/// Switch fullscreen
	////////////////////////////////////////////////////////////
	void ToggleFullscreen()
	{
		bool toggle = !getMainWindow().IsFullscreen();
		getMainWindow().Dispose();
		delete main_window;
		main_window = new WindowUi(System::Width, System::Height, System::Title, true, toggle);
		Graphics::InitOpenGL();
		Graphics::RefreshAll();
	}

	////////////////////////////////////////////////////////////
	/// Resize window
	////////////////////////////////////////////////////////////
	void ResizeWindow(long width, long height)
	{
		getMainWindow().Resize(width, height);
	}

	////////////////////////////////////////////////////////////
	/// Get window width
	////////////////////////////////////////////////////////////
	int GetWidth()
	{
		return getMainWindow().GetWidth();
	}

	////////////////////////////////////////////////////////////
	/// Get window height
	////////////////////////////////////////////////////////////
	int GetHeight()
	{
		return getMainWindow().GetHeight();
	}

	////////////////////////////////////////////////////////////
	/// Swap Buffers
	////////////////////////////////////////////////////////////
	void SwapBuffers()
	{
		getMainWindow().SwapBuffers();
	}
} // namespace Player
