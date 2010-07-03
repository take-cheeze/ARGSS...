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
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>
// #include <conio.h>
#include "output.hxx"
#include "options.hxx"
#include "system.hxx"
#include "player.hxx"
#include "console.hxx"
#include "msgbox.hxx"
#include "graphics.hxx"


namespace Output
{
	////////////////////////////////////////////////////////////
	/// Global Variables
	////////////////////////////////////////////////////////////
	int output_type;
	std::string filename;

	////////////////////////////////////////////////////////////
	/// Output Initialize
	////////////////////////////////////////////////////////////
	void Init()
	{
		output_type = OUTPUT_TYPE;
		filename = OUTPUT_FILE;
	}

	////////////////////////////////////////////////////////////
	/// Output Error
	////////////////////////////////////////////////////////////
	void Error(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);

		char str[256];
	#ifdef MSVC
		vsprintf_s(str, 256, fmt, args);
	#else
		vsprintf(str, fmt, args);
	#endif
		ErrorStr( std::string(str) );

		va_end(args);
	}
	void ErrorStr(std::string const& err)
	{
		PostStr(err);
		if (Console::Active()) {
			Post("\nARGSS will close now. Press any key...");
	#ifdef MSVC
			_getch();
	#else
			std::cin.get();
	#endif
		}
		Player::Exit();
		exit(EXIT_FAILURE);
	}

	////////////////////////////////////////////////////////////
	/// Output Warning
	////////////////////////////////////////////////////////////
	void Warning(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);

		char str[256];
	#ifdef MSVC
		vsprintf_s(str, 256, fmt, args);
	#else
		vsprintf(str, fmt, args);
	#endif
		WarningStr( std::string(str) );

		va_end(args);
	}
	void WarningStr(std::string const& warn)
	{
		PostStr(warn);
	}

	////////////////////////////////////////////////////////////
	/// Output Post message 
	////////////////////////////////////////////////////////////
	void Post(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);

		char str[256];
	#ifdef MSVC
		vsprintf_s(str, 256, fmt, args);
	#else
		vsprintf(str, fmt, args);
	#endif
		PostStr( std::string(str) );

		va_end(args);
	}
	void PostStr(std::string const& msg)
	{
		Graphics::TimerWait();
		switch(output_type) {
		case 1:
			if (Console::Active()) Console::Write(msg);
			break;
		case 2:
			if (!Console::Active()) Console::Init();
			Console::Write(msg);
			break;
		case 3:
			MsgBox::OK(msg, System::getTitle());
			break;
		case 4:
			PostFile(msg);
			break;
		case 5:
			if (Console::Active()) Console::Write(msg);
			else PostFile(msg);
			break;
		case 6:
			if (Console::Active()) Console::Write(msg);
			else MsgBox::OK(msg, System::getTitle());
		}
		Graphics::TimerContinue();
	}

	////////////////////////////////////////////////////////////
	/// Output File
	////////////////////////////////////////////////////////////
	void PostFile(std::string const& msg)
	{
		std::ofstream file;
		file.open(filename.c_str(), std::ios::out | std::ios::app);
		file << msg;
		file.close();
	}

	////////////////////////////////////////////////////////////
	/// Output Console
	////////////////////////////////////////////////////////////
	void Console()
	{
		if (!Console::Active()) Console::Init();
		output_type = 2;
	}

	////////////////////////////////////////////////////////////
	/// Output Console
	////////////////////////////////////////////////////////////
	void MsgBox()
	{
		if (Console::Active()) Console::Free();
		output_type = 3;
	}

	////////////////////////////////////////////////////////////
	/// Output Console
	////////////////////////////////////////////////////////////
	void File(std::string const& name)
	{
		if (Console::Active()) Console::Free();
		output_type = 4;
		filename = name;
	}

	////////////////////////////////////////////////////////////
	/// Output Console
	////////////////////////////////////////////////////////////
	void None()
	{
		if (Console::Active()) Console::Free();
		output_type = 0;
	}

	////////////////////////////////////////////////////////////
	/// Output Console
	////////////////////////////////////////////////////////////
	std::string Gets()
	{
		char string[256] = "\0";
		if (Console::Active()) {
	#ifdef MSVC
			gets_s(string, 256);
	#else
			return std::string( fgets(string, 256, stdin) );
	#endif
		}
		return string;
	}

	////////////////////////////////////////////////////////////
	/// Output Console
	////////////////////////////////////////////////////////////
	std::string Getc()
	{
		std::string chr = "";
		if (Console::Active()) {
	#ifdef MSVC
			chr += _getch();
	#else
			chr += std::cin.get();
	#endif
		}
		return chr;
	}
} // namespace Output
