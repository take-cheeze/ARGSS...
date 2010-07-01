#ifndef _WINDOWUI_QT_H_
#define _WINDOWUI_QT_H_

#include <string>
#include <queue>
#include <QtOpenGL/QGLWidget> // #include "windows.hxx"
#include "event.hxx"
#include "inputkeys.hxx"

class WindowUi : public QGLWidget
{
public:
	WindowUi(long iwidth, long iheight, std::string const& title, bool center, bool fs_flag);
	virtual ~WindowUi();

	void Dispose();
	void SwapBuffers();
	void Resize(long nwidth, long nheight) { resize(nwidth, nheight); }
	void SetTitle(std::string const& title) { setWindowTitle( title.c_str() ); }
	void ToggleFullscreen() { showFullScreen(); }

	long GetWidth() const { return width(); }
	long GetHeight() const { return height(); }

	bool GetEvent(Event& evnt);
	// int ProccesEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool IsFullscreen() const { return isFullScreen(); }
	bool GetMouseFocus() const { return  mouseFocus_; }
	int GetMouseWheel() const { return mouseWheel_; }
	int GetMousePosX() const { return mouseX_; }
	int GetMousePosY() const { return mouseY_; }

	std::vector< bool >& GetKeyStates() { return keys_; }
	std::vector< bool > const& GetKeyStates() const { return keys_; }
private:
	Input::Keys::InputKey QtK2IK(QKeyEvent* event);

	std::queue< Event > events_;
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

	virtual void updateGL();
	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
}; // class WindowUi

#endif
