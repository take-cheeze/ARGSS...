#ifndef _WINDOWUI_QT_HXX_
#define _WINDOWUI_QT_HXX_

#include <string>
#include <queue>
#include <QtOpenGL/QGLWidget>

#include <event.hxx>
#include <inputkeys.hxx>


class WindowUi : public QGLWidget
{
public:
	WindowUi(long iwidth, long iheight, std::string const& title, bool center, bool fs_flag);
	virtual ~WindowUi();

	void swapBuffers();
	void Resize(long nwidth, long nheight) { resize(nwidth, nheight); }
	void setTitle(std::string const& title) { setWindowTitle( title.c_str() ); }
	void toggleFullscreen() { showFullScreen(); }

	long getWidth() const { return width(); }
	long getHeight() const { return height(); }

	bool popEvent(Event& evnt);

	bool isFullscreen() const { return isFullScreen(); }
	bool getMouseFocus() const { return  mouseFocus_; }
	int getMouseWheel() const { return mouseWheel_; }
	int getMousePosX() const { return mouseX_; }
	int getMousePosY() const { return mouseY_; }

	std::vector< bool >& getKeyStates() { return keys_; }
	std::vector< bool > const& getKeyStates() const { return keys_; }
private:
	Input::Keys::InputKey QtK2IK(QKeyEvent* event);

	std::queue<Event> events_;
	std::vector< bool > keys_;
	bool mouseFocus_;
	int mouseWheel_;
	int mouseX_;
	int mouseY_;
protected:
	virtual bool event(QEvent* event);
	virtual void focusInEvent(QFocusEvent* event);
	virtual void focusOutEvent(QFocusEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

	virtual void paintGL();
	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
}; // class WindowUi

#endif // _WINDOWUI_QT_HXX_
