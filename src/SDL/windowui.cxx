#include "windowui.hxx"
#include "../graphics.hxx"

#include <SDL_opengl.h>

#include <cassert>
#include <cstdlib>

#include <iostream>
#include <stack>


namespace
{
	Input::Keys::InputKey SDLKey2IKey(SDLKey key)
	{
		switch(key) {
			case SDLK_KP0: return Input::Keys::KP0;
			case SDLK_KP1: return Input::Keys::KP1;
			case SDLK_KP2: return Input::Keys::KP2;
			case SDLK_KP3: return Input::Keys::KP3;
			case SDLK_KP4: return Input::Keys::KP4;
			case SDLK_KP5: return Input::Keys::KP5;
			case SDLK_KP6: return Input::Keys::KP6;
			case SDLK_KP7: return Input::Keys::KP7;
			case SDLK_KP8: return Input::Keys::KP8;
			case SDLK_KP9: return Input::Keys::KP9;
			case SDLK_BACKSPACE	: return Input::Keys::BACKSPACE;
			case SDLK_TAB		: return Input::Keys::TAB;
			case SDLK_CLEAR		: return Input::Keys::CLEAR;
			case SDLK_RETURN	: return Input::Keys::RETURN;
			case SDLK_PAUSE		: return Input::Keys::PAUSE;
			case SDLK_ESCAPE	: return Input::Keys::ESCAPE;
			case SDLK_SPACE		: return Input::Keys::SPACE;
			case SDLK_PAGEUP	: return Input::Keys::PGUP;
			case SDLK_PAGEDOWN	: return Input::Keys::PGDN;
			case SDLK_END		: return Input::Keys::ENDS;
			case SDLK_HOME		: return Input::Keys::HOME;
			case SDLK_LEFT		: return Input::Keys::LEFT;
			case SDLK_UP		: return Input::Keys::UP;
			case SDLK_RIGHT		: return Input::Keys::RIGHT;
			case SDLK_DOWN		: return Input::Keys::DOWN;
			case SDLK_PRINT		: return Input::Keys::SNAPSHOT;
			case SDLK_INSERT	: return Input::Keys::INSERT;
			case SDLK_DELETE	: return Input::Keys::DEL;
			case SDLK_LCTRL		: return Input::Keys::LCTRL;
			case SDLK_RCTRL		: return Input::Keys::RCTRL;
			case SDLK_LALT		: return Input::Keys::LALT;
			case SDLK_RALT		: return Input::Keys::RALT;
			case SDLK_LSHIFT	: return Input::Keys::LSHIFT;
			case SDLK_RSHIFT	: return Input::Keys::RSHIFT;
			case SDLK_0			: return Input::Keys::N0;
			case SDLK_1			: return Input::Keys::N1;
			case SDLK_2			: return Input::Keys::N2;
			case SDLK_3			: return Input::Keys::N3;
			case SDLK_4			: return Input::Keys::N4;
			case SDLK_5			: return Input::Keys::N5;
			case SDLK_6			: return Input::Keys::N6;
			case SDLK_7			: return Input::Keys::N7;
			case SDLK_8			: return Input::Keys::N8;
			case SDLK_9			: return Input::Keys::N9;
			case SDLK_a			: return Input::Keys::A;
			case SDLK_b			: return Input::Keys::B;
			case SDLK_c			: return Input::Keys::C;
			case SDLK_d			: return Input::Keys::D;
			case SDLK_e			: return Input::Keys::E;
			case SDLK_f			: return Input::Keys::F;
			case SDLK_g			: return Input::Keys::G;
			case SDLK_h			: return Input::Keys::H;
			case SDLK_i			: return Input::Keys::I;
			case SDLK_j			: return Input::Keys::J;
			case SDLK_k			: return Input::Keys::K;
			case SDLK_l			: return Input::Keys::L;
			case SDLK_m			: return Input::Keys::M;
			case SDLK_n			: return Input::Keys::N;
			case SDLK_o			: return Input::Keys::O;
			case SDLK_p			: return Input::Keys::P;
			case SDLK_q			: return Input::Keys::Q;
			case SDLK_r			: return Input::Keys::R;
			case SDLK_s			: return Input::Keys::S;
			case SDLK_t			: return Input::Keys::T;
			case SDLK_u			: return Input::Keys::U;
			case SDLK_v			: return Input::Keys::V;
			case SDLK_w			: return Input::Keys::W;
			case SDLK_x			: return Input::Keys::X;
			case SDLK_y			: return Input::Keys::Y;
			case SDLK_z			: return Input::Keys::Z;
			case SDLK_KP_MULTIPLY	: return Input::Keys::MULTIPLY;
			case SDLK_KP_PLUS		: return Input::Keys::ADD;
			// case SDLK_KP_SEPARATOR		: return Input::Keys::SEPARATOR;
			case SDLK_KP_MINUS	: return Input::Keys::SUBTRACT;
			// case VK_DECIMAL		: return Input::Keys::DECIMAL;
			case SDLK_KP_DIVIDE		: return Input::Keys::DIVIDE;
			case SDLK_F1			: return Input::Keys::F1;
			case SDLK_F2			: return Input::Keys::F2;
			case SDLK_F3			: return Input::Keys::F3;
			case SDLK_F4			: return Input::Keys::F4;
			case SDLK_F5			: return Input::Keys::F5;
			case SDLK_F6			: return Input::Keys::F6;
			case SDLK_F7			: return Input::Keys::F7;
			case SDLK_F8			: return Input::Keys::F8;
			case SDLK_F9			: return Input::Keys::F9;
			case SDLK_F10			: return Input::Keys::F10;
			case SDLK_F11			: return Input::Keys::F11;
			case SDLK_F12			: return Input::Keys::F12;
			case SDLK_CAPSLOCK		: return Input::Keys::CAPS_LOCK;
			case SDLK_NUMLOCK		: return Input::Keys::NUM_LOCK;
			case SDLK_SCROLLOCK		: return Input::Keys::SCROLL_LOCK;
			default: return Input::Keys::NONE;
		}
	}

	std::stack< WindowUi* > currentWindowUI_;
} // namespace

WindowUi::WindowUi(long iwidth, long iheight, std::string const& title, bool center, bool fs_flag)
: keys_(Input::Keys::KEYS_COUNT, false)
, width_(iwidth), height_(iheight)
, isFullScreen_(fs_flag)
{
	assert( SDL_InitSubSystem(SDL_INIT_VIDEO) == 0 );
	assert( SDL_InitSubSystem(SDL_INIT_TIMER) == 0 );
	assert( SDL_InitSubSystem(SDL_INIT_EVENTTHREAD) == 0 );
	atexit( SDL_Quit );

	int flagRet = 0;
	flagRet = SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	assert(flagRet == 0);
	Uint32 flags = SDL_OPENGL;
	if( isFullscreen() ) flags |= SDL_FULLSCREEN;
	assert( SDL_SetVideoMode(width_, height_, 0, flags) );

	mouseWheel_ = 0;
	mouseX_ = mouseY_ = 0;

	setTitle("ARGSS Player");
	// setCursor();
	// setIcon();

/*
	if(center) {
		QSize centerPos = ( QApplication::desktop()->screenGeometry().size() - frameSize() ) / 2;
		move( QPoint( centerPos.width(), centerPos.height() ) );
	}
 */

	currentWindowUI_.push(this);
}
WindowUi::~WindowUi()
{
	currentWindowUI_.pop();
}

void WindowUi::toggleFullscreen()
{
	isFullScreen_ = isFullScreen_ ? false : true;

	resize(width_, height_);
}
void WindowUi::setTitle(std::string const& name)
{
	SDL_WM_SetCaption( name.c_str(), NULL );
}

void WindowUi::swapBuffers()
{
	// glFlush();
	SDL_GL_SwapBuffers();
}
void WindowUi::resize(long width, long height)
{
	width_ = width; height_ = height;

	int flagRet = 0;
	flagRet = SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	assert(flagRet == 0);
	Uint32 flags = SDL_OPENGL;
	if( isFullscreen() ) flags |= SDL_FULLSCREEN;
	assert( SDL_SetVideoMode(width, height, 0, flags) );

	Graphics::ResizeScreen(width, height);
}

bool WindowUi::getEvent(Event& ev)
{
	SDL_Event sdlEv;
	while( SDL_PollEvent(&sdlEv) ) (void) this->event(sdlEv);

	if( !events_.empty() ) {
		ev = events_.front();
		events_.pop();

		return true;
	} else return false;
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
/*
		case SDL_JOYAXISMOTION:
			break;
		case SDL_JOYBALLMOTION:
			break;
		case SDL_JOYHATMOTION:
			break;
		case SDL_JOYBUTTONDOWN:
			break;
		case SDL_JOYBUTTONUP:
			break;
		case SDL_SYSWMEVENT:
			break;
		case SDL_VIDEORESIZE:
			break;
		case SDL_VIDEOEXPOSE:
			break;
		case SDL_USEREVENT:
			break;
 */
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
