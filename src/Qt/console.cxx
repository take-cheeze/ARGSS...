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
/*
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
 */
#include <iostream>
#include <fstream>
#include "../console.hxx"

////////////////////////////////////////////////////////////
/// Initialize Console
////////////////////////////////////////////////////////////
void Console::Init()
{
}

////////////////////////////////////////////////////////////
/// set console title
////////////////////////////////////////////////////////////
void Console::setTitle(char* title)
{
}

////////////////////////////////////////////////////////////
/// set console max lines
////////////////////////////////////////////////////////////
void Console::setLines(int lines)
{
}

////////////////////////////////////////////////////////////
/// Free console
////////////////////////////////////////////////////////////
void Console::Free()
{
}

////////////////////////////////////////////////////////////
/// get console active status
////////////////////////////////////////////////////////////
bool Console::Active()
{
	return true;
}

////////////////////////////////////////////////////////////
/// Write message to console
////////////////////////////////////////////////////////////
void Console::Write(std::string const& msg)
{
	std::cout << msg << std::endl;
}
