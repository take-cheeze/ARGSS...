#ifndef _MSGBOX_QT_HXX_
#define _MSGBOX_QT_HXX_

#include <string>

namespace MsgBox
{
	void OK(std::string const& msg, std::string const& title);
	void Error(std::string const& msg, std::string const& title);
	void Warning(std::string const& msg, std::string const& title);
} // namespace MsgBox

#endif
