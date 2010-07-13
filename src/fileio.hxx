#ifndef _FILEIO_HXX_
#define _FILEIO_HXX_

#include <stdint.h>

#include <vector>
#include <string>


namespace FileIO
{
	void init();
	std::vector< uint8_t > const& get(std::string const& name);

	bool exists(std::string const& filename);

	// exchanges path separater
	std::string toSlash(std::string const& str);
	std::string toYen(std::string const& str);
} // namespace FileIO

#endif
