#ifndef _WINDOWUI_Qt_HXX_
#define _WINDOWUI_Qt_HXX_

#include <string>
#include <queue>

#include <event.hxx>
#include <inputkeys.hxx>

#include <SDL.h>


class WindowUi
{
public:
	WindowUi(long iwidth, long iheight, std::string const& title, bool center, bool fs_flag);
	virtual ~WindowUi();

	void swapBuffers();
	void resize(long nwidth, long nheight);
	void setTitle(std::string const& title);
	void toggleFullscreen();

	long getWidth() const { return width_; }
	long getHeight() const { return height_; }

	bool getEvent(Event& evnt);

	bool isFullscreen() const { return isFullScreen_; }
	bool getMouseFocus() const { return  mouseFocus_; }
	int getMouseWheel() const { return mouseWheel_; }
	int getMousePosX() const { return mouseX_; }
	int getMousePosY() const { return mouseY_; }

	std::vector< bool >& getKeyStates() { return keys_; }
	std::vector< bool > const& getKeyStates() const { return keys_; }

	bool event(SDL_Event const& event);
private:
	std::queue< Event > events_;
	std::vector< bool > keys_;
	bool mouseFocus_;
	int mouseWheel_;
	int mouseX_;
	int mouseY_;
	long width_, height_;

	bool isFullScreen_;
}; // class WindowUi

#endif
