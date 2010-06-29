//////////////////////////////////////////////////////////////////////////////////
/// ARGSS - Copyright (c) 2009 - 2010, Alejandro Marzini (vgvgf)
///         All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///     * Redistributions of source code must retain the above copyright
///       notice, this list of conditions and the following disclaimer.
///     * Redistributions in binary form must reproduce the above copyright
///       notice, this list of conditions and the following disclaimer in the
///       documentation and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY
/// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
/// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
/// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include "output.h"
#include "system.h"
#include "filefinder.h"
#include "player.h"
#include "graphics.h"
#include "audio.h"
#include "input.h"
#include "argss.h"

#include <cstdlib>

////////////////////////////////////////////////////////////
/// Main
////////////////////////////////////////////////////////////
#ifdef WIN32
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else

#include <QtCore/QThread>
#include <QtGui/QApplication>
#include <iostream>

class MainThread : public QThread
{
public:
	MainThread(QObject* parent = NULL) : QThread(parent) {}
	virtual ~MainThread() {}
protected:
	virtual void run()
	{
		ARGSS::Init();
	}
};

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

#endif

	Output::Init();
	System::Init();
	FileFinder::Init();
	Player::Init();
	Graphics::Init();
	Input::Init();
	Audio::Init();

#ifdef WIN32
	ARGSS::Init();
	return EXIT_SUCCESS;
#else
	MainThread mainThread(&app);
	mainThread.start();
	return app.exec();
#endif
}