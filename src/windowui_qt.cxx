#include "windowui_qt.hxx"
#include "graphics.hxx"

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QMouseEvent>


WindowUi::WindowUi(long iwidth, long iheight, std::string const& title, bool center, bool fs_flag)
: QGLWidget()
, keys_(Input::Keys::KEYS_COUNT, false)
{
	resize(iwidth, iheight);
	mouseWheel_ = 0;
	mouseX_ = mouseY_ = 0;

	setWindowTitle("ARGSS Player");
	setCursor( QCursor(Qt::ArrowCursor) );
	// setIcon();

	if(fs_flag) ToggleFullscreen();
	else if(center) {
		QSize centerPos = ( QApplication::desktop()->screenGeometry().size() - frameSize() ) / 2;
		move( QPoint( centerPos.width(), centerPos.height() ) );
	}

	setAutoFillBackground(true);
	setAutoBufferSwap(true);
	makeCurrent();
	setVisible(true);
}
WindowUi::~WindowUi()
{
}

void WindowUi::SwapBuffers()
{
	if( !autoBufferSwap() ) swapBuffers();
}

void WindowUi::Dispose()
{
	setVisible(false);
}
bool WindowUi::GetEvent(Event& ev)
{
	if( !events_.empty() ) {
		ev = events_.front();
		events_.pop();

		return true;
	} else return false;
}

void WindowUi::updateGL()
{
}
void WindowUi::initializeGL()
{
	// Graphics::InitOpenGL();
}
void WindowUi::resizeGL(int width, int height)
{
	Graphics::ResizeScreen(width, height);
}

Input::Keys::InputKey WindowUi::QtK2IK(QKeyEvent* event)
{
	if( event->modifiers() & Qt::KeypadModifier ) switch( event->key() ) {
		case Qt::Key_0: return Input::Keys::KP0;
		case Qt::Key_1: return Input::Keys::KP1;
		case Qt::Key_2: return Input::Keys::KP2;
		case Qt::Key_3: return Input::Keys::KP3;
		case Qt::Key_4: return Input::Keys::KP4;
		case Qt::Key_5: return Input::Keys::KP5;
		case Qt::Key_6: return Input::Keys::KP6;
		case Qt::Key_7: return Input::Keys::KP7;
		case Qt::Key_8: return Input::Keys::KP8;
		case Qt::Key_9: return Input::Keys::KP9;
	} else if( event->modifiers() == Qt::NoModifier ) switch( event->key() ) {
		case Qt::Key_Backspace	: return Input::Keys::BACKSPACE;
		case Qt::Key_Tab		: return Input::Keys::TAB;
		case Qt::Key_Clear		: return Input::Keys::CLEAR;
		case Qt::Key_Return		: return Input::Keys::RETURN;
		case Qt::Key_Pause		: return Input::Keys::PAUSE;
		case Qt::Key_Escape		: return Input::Keys::ESCAPE;
		case Qt::Key_Space		: return Input::Keys::SPACE;
		case Qt::Key_PageUp		: return Input::Keys::PGUP;
		case Qt::Key_PageDown	: return Input::Keys::PGDN;
		case Qt::Key_End		: return Input::Keys::ENDS;
		case Qt::Key_Home		: return Input::Keys::HOME;
		case Qt::Key_Left		: return Input::Keys::LEFT;
		case Qt::Key_Up			: return Input::Keys::UP;
		case Qt::Key_Right		: return Input::Keys::RIGHT;
		case Qt::Key_Down		: return Input::Keys::DOWN;
		case Qt::Key_ScreenSaver: return Input::Keys::SNAPSHOT;
		case Qt::Key_Insert		: return Input::Keys::INSERT;
		case Qt::Key_Delete		: return Input::Keys::DEL;
		case Qt::Key_Shift		: return Input::Keys::SHIFT;
/*
		case Qt::VK_LCONTROL	: return Input::Keys::LCTRL;
		case Qt::VK_RCONTROL	: return Input::Keys::RCTRL;
		case Qt::VK_LMENU		: return Input::Keys::LALT;
		case Qt::VK_RMENU		: return Input::Keys::RALT;
		case Qt::VK_LSHIFT		: return Input::Keys::LSHIFT;
		case Qt::VK_RSHIFT		: return Input::Keys::RSHIFT;
 */
		case Qt::Key_Control	: return Input::Keys::CTRL;
		case Qt::Key_Alt		: return Input::Keys::ALT;
		case Qt::Key_0			: return Input::Keys::N0;
		case Qt::Key_1			: return Input::Keys::N1;
		case Qt::Key_2			: return Input::Keys::N2;
		case Qt::Key_3			: return Input::Keys::N3;
		case Qt::Key_4			: return Input::Keys::N4;
		case Qt::Key_5			: return Input::Keys::N5;
		case Qt::Key_6			: return Input::Keys::N6;
		case Qt::Key_7			: return Input::Keys::N7;
		case Qt::Key_8			: return Input::Keys::N8;
		case Qt::Key_9			: return Input::Keys::N9;
		case Qt::Key_A			: return Input::Keys::A;
		case Qt::Key_B			: return Input::Keys::B;
		case Qt::Key_C			: return Input::Keys::C;
		case Qt::Key_D			: return Input::Keys::D;
		case Qt::Key_E			: return Input::Keys::E;
		case Qt::Key_F			: return Input::Keys::F;
		case Qt::Key_G			: return Input::Keys::G;
		case Qt::Key_H			: return Input::Keys::H;
		case Qt::Key_I			: return Input::Keys::I;
		case Qt::Key_J			: return Input::Keys::J;
		case Qt::Key_K			: return Input::Keys::K;
		case Qt::Key_L			: return Input::Keys::L;
		case Qt::Key_M			: return Input::Keys::M;
		case Qt::Key_N			: return Input::Keys::N;
		case Qt::Key_O			: return Input::Keys::O;
		case Qt::Key_P			: return Input::Keys::P;
		case Qt::Key_Q			: return Input::Keys::Q;
		case Qt::Key_R			: return Input::Keys::R;
		case Qt::Key_S			: return Input::Keys::S;
		case Qt::Key_T			: return Input::Keys::T;
		case Qt::Key_U			: return Input::Keys::U;
		case Qt::Key_V			: return Input::Keys::V;
		case Qt::Key_W			: return Input::Keys::W;
		case Qt::Key_X			: return Input::Keys::X;
		case Qt::Key_Y			: return Input::Keys::Y;
		case Qt::Key_Z			: return Input::Keys::Z;
/*
		case Qt::VK_LWIN		: return Input::Keys::LOS;
		case Qt::VK_RWIN		: return Input::Keys::ROS;
		case Qt::VK_APPS		: return Input::Keys::APPS;
		case Qt::Key_Multiply	: return Input::Keys::MULTIPLY;
		case Qt::Key_Add			: return Input::Keys::ADD;
		case Qt::Key_Separator	: return Input::Keys::SEPARATOR;
		case Qt::Key_Subtract	: return Input::Keys::SUBTRACT;
		case Qt::VK_DECIMAL		: return Input::Keys::DECIMAL;
		case Qt::Key_Divide		: return Input::Keys::DIVIDE;
 */
		case Qt::Key_F1			: return Input::Keys::F1;
		case Qt::Key_F2			: return Input::Keys::F2;
		case Qt::Key_F3			: return Input::Keys::F3;
		case Qt::Key_F4			: return Input::Keys::F4;
		case Qt::Key_F5			: return Input::Keys::F5;
		case Qt::Key_F6			: return Input::Keys::F6;
		case Qt::Key_F7			: return Input::Keys::F7;
		case Qt::Key_F8			: return Input::Keys::F8;
		case Qt::Key_F9			: return Input::Keys::F9;
		case Qt::Key_F10		: return Input::Keys::F10;
		case Qt::Key_F11		: return Input::Keys::F11;
		case Qt::Key_F12		: return Input::Keys::F12;
		case Qt::Key_CapsLock	: return Input::Keys::CAPS_LOCK;
		case Qt::Key_NumLock	: return Input::Keys::NUM_LOCK;
		case Qt::Key_ScrollLock	: return Input::Keys::SCROLL_LOCK;
	}
	return Input::Keys::InputKey(0);
}

void WindowUi::wheelEvent(QWheelEvent* event)
{
	mouseWheel_ = event->delta();
}
void WindowUi::keyPressEvent(QKeyEvent* event)
{
	Event evnt;
	evnt.type = Event::KeyDown;
	Input::Keys::InputKey key = QtK2IK(event); 
	evnt.param1 = key;
	events_.push(evnt);
	keys_[key] = true;
}
void WindowUi::keyReleaseEvent(QKeyEvent* event)
{
	Event evnt;
	evnt.type = Event::KeyUp;
	Input::Keys::InputKey key = QtK2IK(event); 
	evnt.param1 = key;
	events_.push(evnt);
	keys_[key] = false;
}
void WindowUi::mouseMoveEvent(QMouseEvent* event)
{
	mouseX_ = event->x();
	mouseY_ = event->y();
}
void WindowUi::mousePressEvent(QMouseEvent* event)
{
	Qt::MouseButtons but = event->buttons();
	if(but & Qt::LeftButton) keys_[Input::Keys::MOUSE_LEFT] = true;
	if(but & Qt::RightButton) keys_[Input::Keys::MOUSE_RIGHT] = true;
	if(but & Qt::MidButton) keys_[Input::Keys::MOUSE_MIDDLE] = true;
	if(but & Qt::XButton1) keys_[Input::Keys::MOUSE_XBUTTON1] = true;
	if(but & Qt::XButton2) keys_[Input::Keys::MOUSE_XBUTTON2] = true;
}
void WindowUi::mouseReleaseEvent(QMouseEvent* event)
{
	Qt::MouseButtons but = event->buttons();
	if(but & Qt::LeftButton) keys_[Input::Keys::MOUSE_LEFT] = false;
	if(but & Qt::RightButton) keys_[Input::Keys::MOUSE_RIGHT] = false;
	if(but & Qt::MidButton) keys_[Input::Keys::MOUSE_MIDDLE] = false;
	if(but & Qt::XButton1) keys_[Input::Keys::MOUSE_XBUTTON1] = false;
	if(but & Qt::XButton2) keys_[Input::Keys::MOUSE_XBUTTON2] = false;
}
void WindowUi::focusInEvent(QFocusEvent* event)
{
	Event evnt;
	evnt.type = Event::GainFocus;
	events_.push(evnt);

	if( event->reason() == Qt::MouseFocusReason ) mouseFocus_ = true;
}
void WindowUi::focusOutEvent(QFocusEvent* event)
{
	Event evnt;
	evnt.type = Event::LostFocus;
	events_.push(evnt);

	if( event->reason() == Qt::MouseFocusReason ) mouseFocus_ = false;
}
bool WindowUi::event(QEvent* event)
{
	Event evnt;
	bool ret = true;
	switch ( event->type() ) {
/*
		case QEvent::WindowActivate:
		case QEvent::FocusIn:
			evnt.type = Event::GainFocus;
			events_.push(evnt);
			break;
		case QEvent::WindowDeactivate:
		case QEvent::FocusOut:
			evnt.type = Event::LostFocus;
			events_.push(evnt);
			break;
 */
		case QEvent::Close:
			evnt.type = Event::Quit;
			events_.push(evnt);
			break;
		default:
			ret =false;
			break;
	}
	return ret && QGLWidget::event(event);
}
