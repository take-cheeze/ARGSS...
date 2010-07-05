#ifndef _INPUTBUTTONS_QT_HXX_
#define _INPUTBUTTONS_QT_HXX_

#include <map>
#include <vector>

namespace Input
{
	void InitButtons();

	std::map< int, std::vector<int> >& getButtons();
} // namespace Input

#endif
