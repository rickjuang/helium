/*************************************************************
 *	EventLoop
 *	by Rick Juang, 2011/06/20
 *	OS dependent event loop
 *************************************************************/

#include <engine/defines.h>
#include <engine/game_app.h>

#include <system/event_loop.h>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>		// for message pump
//	#include <MMSystem.h>
#endif

namespace System
{

void EventLoop(Engine::GameApp* gameApp)
{
//Pre-defined OS macros: http://sourceforge.net/p/predef/wiki/OperatingSystems/

#ifdef _WIN32
	// using Robert Dunlop's message pump implementation:
	// http://www.mvps.org/directx/articles/writing_the_game_loop.htm

	MSG msg;
	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
	while (gameApp->GetAppState() != GameApp::QUIT && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			gameApp->Update();
		}
	}
//#elif __ANDROID__ //Android is based on Linux and both __ANDROID__ and __linux__ are defined, therefore we test __ANDROID__ first.
//#elif __linux__
//#elif __FreeBSD__
//#elif __APPLE__
//	#if TARGET_OS_IPHONE			// iOS device
//	#elif TARGET_IPHONE_SIMULATOR	// iOS Simulator
//	#elif TARGET_OS_MAC
#else
	while (gameApp->GetAppState() != Engine::GameApp::QUIT)
	{
		gameApp->Update();
	}
#endif
}

}	// Namespace System
