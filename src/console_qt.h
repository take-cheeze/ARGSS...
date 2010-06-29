#ifndef _CONSOLE_QT_H_
#define _CONSOLE_QT_H_

#include <string>

namespace Console
{
	void Init();
	void SetTitle(char* title);
	void SetLines(int lines);
	void Free();
	bool Active();
	void Write(std::string const& msg);
} // namespace Console

#endif