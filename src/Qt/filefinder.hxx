
#ifndef _FILEFINDER_QT_HXX_
#define _FILEFINDER_QT_HXX_

#include <string>

namespace FileFinder
{
	void Init();
	std::string FindImage(std::string const& name);
	std::string FindMusic(std::string const& name);
	std::string FindFont(std::string const& name);
} // namespace FileFinder

#endif
