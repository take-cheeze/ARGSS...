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
#ifdef MINGW
	#define _WIN32_WINNT 0x0500
#endif
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#ifdef _MSC_VER
	#include <io.h>
#else
	#define _fdopen fdopen
	#include <sys/io.h>
#endif
#include <iostream>
#include <fstream>
#include "../console.hxx"

////////////////////////////////////////////////////////////
/// Initialize Console
////////////////////////////////////////////////////////////
void Console::Init()
{
	::AllocConsole();
	long lStdHandle = (long)::GetStdHandle(STD_INPUT_HANDLE);
	int hConHandle =
	#ifdef WIN32
		::_open_osfhandle(lStdHandle, _O_TEXT);
	#else
		0;
	#endif
	FILE* fp = ::_fdopen( hConHandle, "r");
	*stdin = *fp;
	::setvbuf(stdin, NULL, _IONBF, 0);
	Sleep(10);
}

////////////////////////////////////////////////////////////
/// set console title
////////////////////////////////////////////////////////////
void Console::setTitle(char* title)
{
	TCHAR* text = (TCHAR*)(title);
	SetConsoleTitle(text);
}

////////////////////////////////////////////////////////////
/// set console max lines
////////////////////////////////////////////////////////////
void Console::setLines(int lines)
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = lines;
	::SetConsoleScreenBufferSize(::GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
}

////////////////////////////////////////////////////////////
/// Free console
////////////////////////////////////////////////////////////
void Console::Free()
{
	::FreeConsole();
}

////////////////////////////////////////////////////////////
/// get console active status
////////////////////////////////////////////////////////////
bool Console::Active()
{
	return ::GetConsoleWindow() != NULL;
}

////////////////////////////////////////////////////////////
/// Write message to console
////////////////////////////////////////////////////////////
void Console::Write(std::string const& msg)
{
	unsigned int n;
	HANDLE out;
	out = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::WriteConsoleA(out, msg.c_str(), msg.size(), &n, NULL);
}
