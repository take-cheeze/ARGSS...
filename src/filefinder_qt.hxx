
#ifndef _FILEFINDER_QT_H_
#define _FILEFINDER_QT_H_

#include <string>

namespace FileFinder
{
	void Init();
	std::string FindImage(std::string const& name);
	std::string FindMusic(std::string const& name);
	std::string FindFont(std::string const& name);

/*
	extern std::string fonts_path;
	extern std::string rtp_paths[3];
 */
} // namespace FileFinder

#endif
