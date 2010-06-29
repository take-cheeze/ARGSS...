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
#include "filefinder_qt.h"
#include "system.h"
// #include "registry.h"
#include "options.h"

////////////////////////////////////////////////////////////
/// Global Variables
////////////////////////////////////////////////////////////
std::string FileFinder::rtp_paths[3];
std::string FileFinder::fonts_path;

////////////////////////////////////////////////////////////
/// Find image
////////////////////////////////////////////////////////////
void FileFinder::Init() {
/*
	for (int i = 0; i < 3; i++) {
		if (System::RTPS[i] == "") continue;
		if (RPGMAKER == RPGXP) {
			rtp_paths[i] = Registry::ReadStrValue(HKEY_CURRENT_USER, "SOFTWARE\\Enterbrain\\RGSS\\RTP", System::RTPS[i]);
			if (rtp_paths[i].size() == 0) {
				rtp_paths[i] = Registry::ReadStrValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Enterbrain\\RGSS\\RTP", System::RTPS[i]);
			}
		}
		else if (RPGMAKER == RPGVX) {
			rtp_paths[i] = Registry::ReadStrValue(HKEY_CURRENT_USER, "SOFTWARE\\Enterbrain\\RGSS2\\RTP", System::RTPS[i]);
			if (rtp_paths[i].size() == 0) {
				rtp_paths[i] = Registry::ReadStrValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Enterbrain\\RGSS2\\RTP", System::RTPS[i]);
			}
		}
	}
 */
	fonts_path = "";
/*
#if MSVC
	wchar_t* dir = new wchar_t[256];
#else
	char* dir  = new char[256];
#endif
	int n = GetWindowsDirectory(dir, 256);
	if (n > 0) {
		char* str = (char*)dir;
		for(unsigned int i = 0; i < n * sizeof(dir[0]); i++) {
			if (str[i] != '\0' ) {
				fonts_path += str[i];
			}
		}
		fonts_path += "\\Fonts\\";
		std::string string_value = fonts_path;
	}
	delete dir;
 */
}

////////////////////////////////////////////////////////////
/// Check if file exists
////////////////////////////////////////////////////////////
bool fexists(std::string const& filename) {
	std::ifstream file(filename.c_str());
	return file.is_open();
}

////////////////////////////////////////////////////////////
/// Make path
////////////////////////////////////////////////////////////
std::string slasher(std::string const& str) {
	std::string ret;
	for(std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		ret.push_back( (*it == '\\') ? '/' : *it );
	}
	return ret;
}

////////////////////////////////////////////////////////////
/// Find image
////////////////////////////////////////////////////////////
std::string FileFinder::FindImage(std::string const& name) {
	std::string target = slasher(name), path = target;
	if (fexists(path)) return path;
	path = target; path += ".bmp";
	if (fexists(path)) return path;
	path = target; path += ".gif";
	if (fexists(path)) return path;
	path = target; path += ".jpg";
	if (fexists(path)) return path;
	path = target; path += ".jpeg";
	if (fexists(path)) return path;
	path = target; path += ".png";
	if (fexists(path)) return path;
	for (int i = 0; i < 3; i++) {
		if (rtp_paths[i] != "") {
			std::string rtp_path = slasher(rtp_paths[i]);
			rtp_path += target;
			path = rtp_path;
			if (fexists(path)) return path;
			path = rtp_path; path += ".bmp";
			if (fexists(path)) return path;
			path = rtp_path; path += ".gif";
			if (fexists(path)) return path;
			path = rtp_path; path += ".jpg";
			if (fexists(path)) return path;
			path = rtp_path; path += ".jpeg";
			if (fexists(path)) return path;
			path = rtp_path; path += ".png";
			if (fexists(path)) return path;
		}
	}
	return "";
}

////////////////////////////////////////////////////////////
/// Find music
////////////////////////////////////////////////////////////
std::string FileFinder::FindMusic(std::string const& name) {
	std::string target = slasher(name), path = target;
	if (fexists(path)) return path;
	path = target; path += ".wav";
	if (fexists(path)) return path;
	path = target; path += ".mid";
	if (fexists(path)) return path;
	path = target; path += ".midi";
	if (fexists(path)) return path;
	path = target; path += ".ogg";
	if (fexists(path)) return path;
	path = target; path += ".mp3";
	if (fexists(path)) return path;
	for (int i = 0; i < 3; i++) {
		if (rtp_paths[i] != "") {
			std::string rtp_path = slasher(rtp_paths[i]);
			rtp_path += target;
			path = rtp_path;
			if (fexists(path)) return path;
			path = rtp_path; path += ".wav";
			if (fexists(path)) return path;
			path = rtp_path; path += ".mid";
			if (fexists(path)) return path;
			path = rtp_path; path += ".midi";
			if (fexists(path)) return path;
			path = rtp_path; path += ".ogg";
			if (fexists(path)) return path;
			path = rtp_path; path += ".mp3";
			if (fexists(path)) return path;
		}
	}
	return "";
}

////////////////////////////////////////////////////////////
/// Find font
////////////////////////////////////////////////////////////
std::string FileFinder::FindFont(std::string const& name) {
	std::string target = slasher(name), path = target;
	if (fexists(path)) return path;
	path = target; path += ".ttf";
	if (fexists(path)) return path;

	path = fonts_path; path += target;
	if (fexists(path)) return path;
	path = fonts_path; path += target; path += ".ttf";
	if (fexists(path)) return path;
/*
	std::string real_target;
	real_target = Registry::ReadStrValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", target + " (TrueType)");
	if (real_target.length() > 0) {
		path = real_target;
		if (fexists(path)) return path;
		path = fonts_path; path += real_target;
		if (fexists(path)) return path;
	}

	real_target = Registry::ReadStrValue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Fonts", target + " (TrueType)");
	if (real_target.length() > 0) {
		path = real_target;
		if (fexists(path)) return path;
		path = fonts_path; path += real_target;
		if (fexists(path)) return path;
	}
 */
	return "";
}
