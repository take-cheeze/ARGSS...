#ifndef _CONSOLE_QT_HXX_
#define _CONSOLE_QT_HXX_

#include <string>

namespace Console
{
	void Init();
	void setTitle(char* title);
	void setLines(int lines);
	void Free();
	bool Active();
	void Write(std::string const& msg);
} // namespace Console

#endif
