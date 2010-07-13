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
#include <vector>
#include <stdexcept>

#include "../filefinder.hxx"
#include "../fileio.hxx"
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

		typedef boost::array< char const*, 5 > ImageSuffix;
		typedef boost::array< char const*, 5 > MusicSuffix;
		typedef boost::array< char const*, 4 >  FontSuffix;
		ImageSuffix imageSuffix_ = { { ".bmp", ".gif", ".jpg", ".jpeg", ".png", } };
		MusicSuffix musicSuffix_ = { { ".wav", ".mid", ".midi", ".ogg", ".mp3", } };
		 FontSuffix  fontSuffix_ = { { ".ttf", ".otf", ".tfb", ".fnt", } };
	}

	////////////////////////////////////////////////////////////
	/// Find image
	////////////////////////////////////////////////////////////
	void Init()
	{
		FileIO::init();

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
	std::vector< uint8_t > const& FindImage(std::string const& name)
	{
		std::string target = FileIO::toSlash(name), path = target;

		if (FileIO::exists(path)) return FileIO::get(path);
		for(ImageSuffix::const_iterator it = imageSuffix_.begin(); it < imageSuffix_.end(); ++it) {
			path = target + *it;
			if (FileIO::exists(path)) return FileIO::get(path);
		}
		for (int i = 0; i < 3; i++) {
			if ( !rtp_paths[i].empty() ) {
				std::string rtp_path = FileIO::toSlash(rtp_paths[i]) + target;
				path = rtp_path;
				if (FileIO::exists(path)) return FileIO::get(path);
				for(ImageSuffix::const_iterator it = imageSuffix_.begin(); it < imageSuffix_.end(); ++it) {
					path = rtp_path + target + *it;
					if (FileIO::exists(path)) return FileIO::get(path);
				}
			}
		}

		throw std::runtime_error("file not found");
	}

	////////////////////////////////////////////////////////////
	/// Find music
	////////////////////////////////////////////////////////////
	std::vector< uint8_t > const& FindMusic(std::string const& name)
	{
		std::string target = FileIO::toSlash(name), path = target;

		if (FileIO::exists(path)) return FileIO::get(path);
		for(MusicSuffix::const_iterator it = musicSuffix_.begin(); it < musicSuffix_.end(); ++it) {
			path = target + *it;
			if (FileIO::exists(path)) return FileIO::get(path);
		}
		for (int i = 0; i < 3; i++) {
			if ( rtp_paths[i].empty() ) {
				std::string rtp_path = FileIO::toSlash(rtp_paths[i]);
				rtp_path += target;
				path = rtp_path;
				if (FileIO::exists(path)) return FileIO::get(path);
				for(MusicSuffix::const_iterator it = musicSuffix_.begin(); it < musicSuffix_.end(); ++it) {
					path = rtp_path + target + *it;
					if (FileIO::exists(path)) return FileIO::get(path);
				}
			}
		}

		throw std::runtime_error("file not found");
	}

	////////////////////////////////////////////////////////////
	/// Find font
	////////////////////////////////////////////////////////////
	std::vector< uint8_t > const& FindFont(std::string const& name)
	{
		std::string target = FileIO::toSlash(name), path = target;
		if (FileIO::exists(path)) return FileIO::get(path);
		path = target + ".ttf";
		if (FileIO::exists(path)) return FileIO::get(path);

		path = fonts_path + target;
		if (FileIO::exists(path)) return FileIO::get(path);
		path = fonts_path + target + ".ttf";
		if (FileIO::exists(path)) return FileIO::get(path);
		std::string real_target;
		real_target = Registry::ReadStrValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", target + " (TrueType)");
		if (real_target.length() > 0) {
			path = real_target;
			if (FileIO::exists(path)) return FileIO::get(path);
			path = fonts_path; path += real_target;
			if (FileIO::exists(path)) return FileIO::get(path);
		}

		real_target = Registry::ReadStrValue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Fonts", target + " (TrueType)");
		if (real_target.length() > 0) {
			path = real_target;
			if (FileIO::exists(path)) return FileIO::get(path);
			path = fonts_path; path += real_target;
			if (FileIO::exists(path)) return FileIO::get(path);
		}

		throw std::runtime_error("file not found");
	}

	std::vector< uint8_t > const& FindFile(std::string const& name)
	{
		if( FileIO::exists(name) ) return FileIO::get(name);

		throw std::runtime_error("file not found");
	}
} // namespace FileFinder
