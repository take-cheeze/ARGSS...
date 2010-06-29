#ifndef _MSGBOX_QT_H_
#define _MSGBOX_QT_H_

#include <string>

namespace MsgBox
{
	void OK(std::string const& msg, std::string const& title);
	void Error(std::string const& msg, std::string const& title);
	void Warning(std::string const& msg, std::string const& title);
} // namespace MsgBox

#endif
