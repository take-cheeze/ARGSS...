#ifndef _INPUTBUTTONS_QT_H_
#define _INPUTBUTTONS_QT_H_

#include <map>
#include <vector>

namespace Input
{
	void InitButtons();

	extern std::map< int, std::vector<int> > buttons;
} // namespace Input

#endif
