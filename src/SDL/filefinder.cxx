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

#include <cstdlib>

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
		boost::array< std::string, 4 > searchPath_;
		std::string fonts_path;

		#if (RPGMAKER == RPGXP)
			char const* RTP_BASE_PATH = ".wine/drive_c/Program Files/Common Files/Enterbrain/RGSS";
			char const* REGISTRY_NAME = "SOFTWARE\\Enterbrain\\RGSS\\RTP";
		#elif (RPGMAKER == RPGVX)
			char const* RTP_BASE_PATH = ".wine/drive_c/Program Files/Common Files/Enterbrain/RGSS2";
			char const* REGISTRY_NAME = "SOFTWARE\\Enterbrain\\RGSS2\\RTP";
		#else
			#error unknown RGSS version
		#endif

		typedef boost::array< char const*, 5 > ImageSuffix;
		typedef boost::array< char const*, 10 > MusicSuffix;
		typedef boost::array< char const*, 4 >  FontSuffix;
		ImageSuffix imageSuffix_ = { { ".bmp", ".gif", ".jpg", ".jpeg", ".png", } };
		MusicSuffix musicSuffix_ = { { ".wav", ".mid", ".midi", ".ogg", ".mp3", ".WAV", ".MID", ".MIDI", ".OGG", ".MP3", } };
		 FontSuffix  fontSuffix_ = { { ".ttf", ".otf", ".tfb", ".fnt", } };
	}

	void Init()
	{
		FileIO::init();

		searchPath_[0].clear(); // current directory

		char const* homePath = std::getenv("HOME");
		if( homePath ) for(int i = 0; i < 3; i++) {
			std::string const& rtp = System::getRTP(i);
			if( !rtp.empty() ) searchPath_[i + 1].assign(homePath).append("/").append(RTP_BASE_PATH).append("/").append(rtp).append("/");
		} else for (int i = 0; i < 3; i++) {
			std::string const& rtp = System::getRTP(i);
			if (rtp.empty()) continue;

			searchPath_[i + 1] = Registry::ReadStrValue(HKEY_CURRENT_USER, REGISTRY_NAME, rtp);
			if ( !searchPath_[i + 1].empty() ) {
				searchPath_[i + 1] = Registry::ReadStrValue(HKEY_LOCAL_MACHINE, REGISTRY_NAME, rtp);
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
		std::string target = FileIO::toSlash(name);

		if(FileIO::exists(target)) return FileIO::get(target);
		for(unsigned int i = 0; i < searchPath_.size(); i++) {
			if( i && searchPath_[i].empty() ) continue;

			std::string base(searchPath_[i]); base.append(target);
			if( FileIO::exists(base) ) return FileIO::get(base);
			for(ImageSuffix::const_iterator it = imageSuffix_.begin(); it < imageSuffix_.end(); ++it) {
				std::string full(base); full.append(*it);
				if (FileIO::exists(full)) return FileIO::get(full);
			}
		}

		throw std::runtime_error("file not found:" + name);
	}

	////////////////////////////////////////////////////////////
	/// Find music
	////////////////////////////////////////////////////////////
	std::vector< uint8_t > const& FindMusic(std::string const& name)
	{
		std::string target = FileIO::toSlash(name);

		if(FileIO::exists(target)) return FileIO::get(target);
		for(unsigned int i = 0; i < searchPath_.size(); i++) {
			if( i && searchPath_[i].empty() ) continue;

			std::string base(searchPath_[i]); base.append(target);
			if( FileIO::exists(base) ) return FileIO::get(base);
			for(MusicSuffix::const_iterator it = musicSuffix_.begin(); it < musicSuffix_.end(); ++it) {
				std::string full(base); full.append(*it);
				if (FileIO::exists(full)) return FileIO::get(full);
			}
		}

		throw std::runtime_error("file not found:" + name);
	}

	////////////////////////////////////////////////////////////
	/// Find font
	////////////////////////////////////////////////////////////
	std::vector< uint8_t > const& FindFont(std::string const& name)
	{
		std::string target = FileIO::toSlash(name), path = target;
		if (FileIO::exists(path)) return FileIO::get(path);
		path = target + fontSuffix_[0];
		if (FileIO::exists(path)) return FileIO::get(path);

		path.assign(fonts_path).append(target);
		if (FileIO::exists(path)) return FileIO::get(path);
		path.assign(fonts_path).append(target).append(fontSuffix_[0]);
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

		throw std::runtime_error("file not found:" + name);
	}

	std::vector< uint8_t > const& FindFile(std::string const& name)
	{
		if( FileIO::exists(name) ) return FileIO::get(name);

		throw std::runtime_error("file not found:" + name);
	}
} // namespace FileFinder
