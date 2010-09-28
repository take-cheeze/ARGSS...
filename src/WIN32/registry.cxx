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
#include <string>

#include "registry.hxx"


namespace Registry
{
	namespace
	{
		typedef std::basic_string< WCHAR > WCharString;

		////////////////////////////////////////////////////////////
		/// Unicode std::string to LPCWSTR
		////////////////////////////////////////////////////////////
		WCharString s2ws2(std::string const& s)
		{
			int len;
			int slength = (int)s.length() + 1;
			len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
			boost::scoped_array< WCHAR > buf( new WCHAR[len] );
			MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf.get(), len);
			WCharString r( buf.get() );
			return r;
		}
	}

	////////////////////////////////////////////////////////////
	/// Read String value
	////////////////////////////////////////////////////////////
	std::string ReadStrValue(HKEY hkey, std::string const& key, std::string const& val)
	{
	#ifdef ARGSS_WIN32
		char value[1024]; 
		DWORD size = 1024;
		DWORD type = REG_SZ;
		HKEY key_handle;

		WCharString wkey = s2ws2(key);
		if (::RegOpenKeyEx(hkey, wkey.c_str(), NULL, KEY_QUERY_VALUE, &key_handle)) {
			return std::string();
		}

		WCharString wval = s2ws2(val);
		if (::RegQueryValueEx(key_handle, wval.c_str(), DWORD(), &type, (LPBYTE)&value, &size)) {
			return std::string();
		}
		::RegCloseKey(key_handle);

		std::string string_value(0);
		for(unsigned int i = 0; i < size; i++) {
			if (value[i] != '\0' ) {
				string_value += value[i];
			}
		}
		return string_value;
	#else
		key.empty(); val.empty();
		return std::string();
	#endif
	}

	////////////////////////////////////////////////////////////
	/// Read Binary value
	////////////////////////////////////////////////////////////
	int ReadBinValue(HKEY hkey, std::string const& key, std::string const& val, uint8_t* bin)
	{
	#ifdef ARGSS_WIN32
		DWORD size = 1024;
		DWORD type = REG_BINARY;
		HKEY key_handle;

		WCharString wkey = s2ws2(key);
		if (::RegOpenKeyEx(hkey, wkey.c_str(), DWORD(), KEY_QUERY_VALUE, &key_handle)) {
			return 0;
		}

		WCharString wval = s2ws2(val);
		if (::RegQueryValueEx(key_handle, wval.c_str(), NULL, &type, bin, &size)) {
			return 0;
		}
		::RegCloseKey(key_handle);

		return size;
	#else
		key.empty(); val.empty(); bin = bin;
		return 0;
	#endif
	}
} // namespace Registry
