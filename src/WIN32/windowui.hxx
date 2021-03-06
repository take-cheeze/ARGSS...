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

#ifndef _WINDOWUI_WIN32_HXX_
#define _WINDOWUI_WIN32_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <queue>
#include <windows.h>

#include <event.hxx>
#include <inputkeys.hxx>

////////////////////////////////////////////////////////////
/// WindowUi class
////////////////////////////////////////////////////////////
class WindowUi
{
public:
	WindowUi(long iwidth, long iheight, std::string title, bool center, bool fs_flag);
	~WindowUi();

	void Dispose();
	void swapBuffers();
	void resize(long nwidth, long nheight);
	void setTitle(std::string title);
	void toggleFullscreen();
	long getWidth();
	long getHeight();

	bool popEvent(Event& evnt);
	int ProccesEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool isFullscreen();
	std::vector<bool> getKeyStates();
	bool getMouseFocus();
	int getMouseWheel();
	int getMousePosX();
	int getMousePosY();

private:
	Input::Keys::InputKey VK2IK(int vk);

	HDC hdc;
	HGLRC hrc;
	HWND hwnd;
	HINSTANCE hinstance;
	long width;
	long height;

	std::queue<Event> events;
	std::vector<bool> keys;
	bool mouse_focus;
	int mouse_wheel;
	int mouse_x;
	int mouse_y;
	bool fullscreen;
}; // class WindowUi

#endif // _WINDOWUI_WIN32_HXX_
