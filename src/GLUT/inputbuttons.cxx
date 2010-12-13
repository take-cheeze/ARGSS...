#include <argss/ruby.hxx>

#include "../inputbuttons.hxx"
#include "../inputkeys.hxx"
// #include "../registry.hxx"
#include "../options.hxx"


namespace Input
{
	namespace
	{
		std::map< int, std::vector<int> > buttons;
	} // namespace

	std::map< int, std::vector<int> >& getButtons() { return buttons; }

	void InitButtons() {
		bool standard = true;

		if (READ_BUTTON_ASSIGMENT) {
		}
		if (standard) {
			buttons[11].push_back(Keys::Z);
			buttons[11].push_back(Keys::SHIFT);
			buttons[12].push_back(Keys::X);
			buttons[12].push_back(Keys::KP0);
			buttons[12].push_back(Keys::ESCAPE);
			buttons[13].push_back(Keys::SPACE);
			buttons[13].push_back(Keys::RETURN);
			buttons[13].push_back(Keys::C);
			buttons[14].push_back(Keys::A);
			buttons[15].push_back(Keys::S);
			buttons[16].push_back(Keys::D);
			buttons[17].push_back(Keys::Q);
			buttons[18].push_back(Keys::W);
			if (JOYSTICK_SUPPORT) {
				buttons[11].push_back(Keys::JOY_1);
				buttons[12].push_back(Keys::JOY_2);
				buttons[13].push_back(Keys::JOY_3);
				buttons[14].push_back(Keys::JOY_4);
				buttons[15].push_back(Keys::JOY_5);
				buttons[16].push_back(Keys::JOY_6);
				buttons[17].push_back(Keys::JOY_7);
				buttons[18].push_back(Keys::JOY_8);
			}
		}
		buttons[2].push_back(Keys::DOWN);
		buttons[4].push_back(Keys::LEFT);
		buttons[6].push_back(Keys::RIGHT);
		buttons[8].push_back(Keys::UP);
		buttons[21].push_back(Keys::SHIFT);
		buttons[22].push_back(Keys::CTRL);
		buttons[23].push_back(Keys::ALT);
		buttons[25].push_back(Keys::F5);
		buttons[26].push_back(Keys::F6);
		buttons[27].push_back(Keys::F7);
		buttons[28].push_back(Keys::F8);
		buttons[29].push_back(Keys::F9);
	}
} // namespace Input
