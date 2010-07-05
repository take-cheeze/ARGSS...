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
#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>

#include <iostream>
#include <fstream>
#include <vector>

#include "filefinder.hxx"
#include "../system.hxx"
#include "../registry.hxx"
#include "../options.hxx"


namespace FileFinder
{
	namespace
	{
		////////////////////////////////////////////////////////////
		/// Global Variables
		////////////////////////////////////////////////////////////
		std::string rtp_paths[3];
		std::string fonts_path;

		template< typename T, int ElmNum >
		int elmNum( T const (&arg)[ElmNum] ) { return ElmNum; }

		typedef boost::array< char const*, 5 > ImageSuffix;
		typedef boost::array< char const*, 5 > MusicSuffix;
		typedef boost::array< char const*, 4 >  FontSuffix;
		ImageSuffix imageSuffix = { { ".bmp", ".gif", ".jpg", ".jpeg", ".png", } };
		MusicSuffix musicSuffix = { { ".wav", ".mid", ".midi", ".ogg", ".mp3", } };
		 FontSuffix fontSuffix = { { ".ttf", ".otf", ".tfb", ".fnt", } };

		////////////////////////////////////////////////////////////
		/// Check if file exists
		////////////////////////////////////////////////////////////
		bool fexists(std::string const& filename)
		{
			std::ifstream file(filename.c_str());
			return file.is_open();
		}

		////////////////////////////////////////////////////////////
		/// Make path
		////////////////////////////////////////////////////////////
		std::string slasher(std::string const& str)
		{
			std::string ret;
			for(std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
				ret.push_back( (*it == '\\') ? '/' : *it );
			}
			return ret;
		}
	}

	////////////////////////////////////////////////////////////
	/// Find image
	////////////////////////////////////////////////////////////
	void Init()
	{
		for (int i = 0; i < 3; i++) {
			std::string const& rtp = System::getRTP(i);
			if (rtp.empty()) continue;
			if (RPGMAKER == RPGXP) {
				rtp_paths[i] = Registry::ReadStrValue(HKEY_CURRENT_USER, "SOFTWARE\\Enterbrain\\RGSS\\RTP", rtp);
				if (rtp_paths[i].size() == 0) {
					rtp_paths[i] = Registry::ReadStrValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Enterbrain\\RGSS\\RTP", rtp);
				}
			}
			else if (RPGMAKER == RPGVX) {
				rtp_paths[i] = Registry::ReadStrValue(HKEY_CURRENT_USER, "SOFTWARE\\Enterbrain\\RGSS2\\RTP", rtp);
				if (rtp_paths[i].size() == 0) {
					rtp_paths[i] = Registry::ReadStrValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Enterbrain\\RGSS2\\RTP", rtp);
				}
			}
		}
		fonts_path.clear();

		WCHAR dir[256];
		int n = 0; // getWindowsDirectory(dir, 256);
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
	}

	////////////////////////////////////////////////////////////
	/// Find image
	////////////////////////////////////////////////////////////
	std::string FindImage(std::string const& name)
	{
		std::string target = slasher(name), path = target;

		if (fexists(path)) return path;
		for(ImageSuffix::const_iterator it = imageSuffix.begin(); it < imageSuffix.end(); ++it) {
			path = target + *it;
			if (fexists(path)) return path;
		}
		for (int i = 0; i < 3; i++) {
			if ( !rtp_paths[i].empty() ) {
				std::string rtp_path = slasher(rtp_paths[i]) + target;
				path = rtp_path;
				if (fexists(path)) return path;
				for(ImageSuffix::const_iterator it = imageSuffix.begin(); it < imageSuffix.end(); ++it) {
					path = rtp_path + target + *it;
					if (fexists(path)) return path;
				}
			}
		}

		return std::string();
	}

	////////////////////////////////////////////////////////////
	/// Find music
	////////////////////////////////////////////////////////////
	std::string FindMusic(std::string const& name)
	{
		std::string target = slasher(name), path = target;

		if (fexists(path)) return path;
		for(MusicSuffix::const_iterator it = musicSuffix.begin(); it < musicSuffix.end(); ++it) {
			path = target + *it;
			if (fexists(path)) return path;
		}
		for (int i = 0; i < 3; i++) {
			if ( rtp_paths[i].empty() ) {
				std::string rtp_path = slasher(rtp_paths[i]);
				rtp_path += target;
				path = rtp_path;
				if (fexists(path)) return path;
				for(MusicSuffix::const_iterator it = musicSuffix.begin(); it < musicSuffix.end(); ++it) {
					path = rtp_path + target + *it;
					if (fexists(path)) return path;
				}
			}
		}

		return std::string();
	}

	////////////////////////////////////////////////////////////
	/// Find font
	////////////////////////////////////////////////////////////
	std::string FindFont(std::string const& name)
	{
		std::string target = slasher(name), path = target;
		if (fexists(path)) return path;
		path = target + ".ttf";
		if (fexists(path)) return path;

		path = fonts_path + target;
		if (fexists(path)) return path;
		path = fonts_path + target + ".ttf";
		if (fexists(path)) return path;
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

		return std::string();
	}
} // namespace FileFinder
