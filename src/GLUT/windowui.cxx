#include "windowui.hxx"
#include "../graphics.hxx"

#include <GL/glut.h>

#include <cassert>
#include <cstdlib>

#include <iostream>
#include <map>
#include <stack>


namespace
{
	namespace callback
	{
		void specialDown(int const key, int const x, int const y)
		{
		}
	} // namespace 

	std::map<int, WindowUi*> windowUiMap_;
} // namespace

WindowUi::WindowUi(long const iwidth, long const iheight, std::string const& title, bool const center, bool const fs_flag)
: keys_(Input::Keys::KEYS_COUNT, false)
, width_(iwidth), height_(iheight)
, isFullScreen_(fs_flag)
{
	if(center) {
		glutInitWindowPosition( ( glutGet(GLUT_SCREEN_WIDTH) - iwidth ) / 2,
			( glutGet(GLUT_SCREEN_HEIGHT) - iheight ) / 2 );
	} else { glutInitWindowPosition(0, 0); }
	glutInitWindowSize(iwidth, iheight);
	windowHandle_ = glutCreateWindow(title.c_str());

	if(isFullScreen_) {
		glutFullScreen();
	}

	mouseWheel_ = 0;
	mouseX_ = mouseY_ = 0;

	windowUiMap_[windowHandle_] = this;
}
WindowUi::~WindowUi()
{
	glutDestroyWindow(windowHandle_);
}

void WindowUi::toggleFullscreen()
{
	isFullScreen_ = !isFullScreen_;

	glutSetWindow(windowHandle_);
	glutFullScreen();
	resize(width_, height_);
}
void WindowUi::setTitle(std::string const& name)
{
	glutSetWindow(windowHandle_);
	glutSetWindowTitle( const_cast<char*>( name.c_str() ) );
}

void WindowUi::swapBuffers()
{
	glutSetWindow(windowHandle_);
	glutSwapBuffers();
}
void WindowUi::resize(long const w, long const h)
{
	width_ = w; height_ = h;

	glutSetWindow(windowHandle_);
	glutReshapeWindow(w, h);

	Graphics::ResizeScreen(w, h);
}

boost::optional<Event> WindowUi::popEvent()
{
	if( !events_.empty() ) {
		Event ev = events_.front();
		events_.pop();
		return ev;
	} else boost::none;
}

/*
void WindowUi::wheelEvent(QWheelEvent* event)
{
	mouseWheel_ = event->delta();
}
 */
bool WindowUi::event(SDL_Event const& src)
{
	Event dst;
	bool ret = true;
	switch ( src.type ) {
		case SDL_ACTIVEEVENT:
			dst.type = src.active.gain ? Event::GainFocus : Event::LostFocus;
			if(src.active.state == SDL_APPMOUSEFOCUS) mouseFocus_ = src.active.gain ? true : false;
			break;
		case SDL_KEYDOWN: {
			dst.type = Event::KeyDown;
			Input::Keys::InputKey key = SDLKey2IKey(src.key.keysym.sym); 

			if(key == Input::Keys::NONE) {
				ret = false;
				break;
			} else {
				dst.param1 = key;
				keys_[key] = true;
			}
		} break;
		case SDL_KEYUP: {
			dst.type = Event::KeyUp;
			Input::Keys::InputKey key = SDLKey2IKey(src.key.keysym.sym); 

			if(key == Input::Keys::NONE) {
				ret = false;
				break;
			} else {
				dst.param1 = key;
				keys_[key] = false;
			}
		} break;
		case SDL_MOUSEMOTION:
			mouseX_ = src.motion.x;
			mouseY_ = src.motion.y;
			ret = false;
			break;
		case SDL_MOUSEBUTTONDOWN: {
			int but = src.button.button;
			if(but & SDL_BUTTON_LEFT) keys_[Input::Keys::MOUSE_LEFT] = true;
			if(but & SDL_BUTTON_RIGHT) keys_[Input::Keys::MOUSE_RIGHT] = true;
			if(but & SDL_BUTTON_MIDDLE) keys_[Input::Keys::MOUSE_MIDDLE] = true;
			// if(but & Qt::XButton1) keys_[Input::Keys::MOUSE_XBUTTON1] = true;
			// if(but & Qt::XButton2) keys_[Input::Keys::MOUSE_XBUTTON2] = true;
			ret = false;
		} break;
		case SDL_MOUSEBUTTONUP: {
			int but = src.button.button;
			if(but & SDL_BUTTON_LEFT) keys_[Input::Keys::MOUSE_LEFT] = false;
			if(but & SDL_BUTTON_RIGHT) keys_[Input::Keys::MOUSE_RIGHT] = false;
			if(but & SDL_BUTTON_MIDDLE) keys_[Input::Keys::MOUSE_MIDDLE] = false;
			// if(but & Qt::XButton1) keys_[Input::Keys::MOUSE_XBUTTON1] = false;
			// if(but & Qt::XButton2) keys_[Input::Keys::MOUSE_XBUTTON2] = false;
			ret = false;
		} break;
		case SDL_QUIT:
			dst.type = Event::Quit;
			events_.push(dst);
			break;
		default:
			ret = false;
			break;
	}
	if(ret) events_.push(dst);

	return ret;
}
