/*************************************************************
 *	GameApp
 *	by Rick Juang, 2011/06/20
 *	Base Game class
 *************************************************************/

#include <engine/defines.h>
#include <engine/globals.h>
#include <system/event_loop.h>
#include <graphics/base_renderer.h>
#include <utility/debug.h>
#include <utility/container.h>

#ifdef _WIN32
#include <windows.h>		// for message pump
#endif

#include "game_app.h"

namespace Global
{

Engine::GameApp* g_GameApp = NULL;

}	// namespace Global

namespace Engine
{

int GameApp::Main(GameApp* game, int argc, char* argv[])
{
	Global::g_GameApp = game;

	Global::g_GameApp->Init();
	Global::g_GameApp->Start();

	System::EventLoop(Global::g_GameApp);

	Global::g_GameApp->Exit();

	Global::g_GameApp = NULL;

	return 0;
}

GameApp::GameApp(void)
:	m_state		(INIT)
,	m_renderer	(NULL)
{
}

void GameApp::Init(void)
{
	if (m_renderer)
	{
		m_renderer->Init();
	}
	OnInit();
}

void GameApp::Start(void)
{
	m_sysTimer.Reset();

	if (m_renderer)
	{
		m_renderer->Start();
	}
	OnStart();
}

void GameApp::Update(void)
{
	m_state = RUNNING;

	for (STL::list<Utility::Timer*>::iterator it = m_timers.begin(); it != m_timers.end(); ++it)
	{
		(*it)->Update();
	}

	OnUpdate();
}

void GameApp::Exit(void)
{
	OnExit();
	if (m_renderer)
	{
		m_renderer->Shutdown();
	}

	m_timers.clear();
}

void GameApp::RegisterTimer(Utility::Timer* timer)
{
	if (STL::find(m_timers.begin(), m_timers.end(), timer) == m_timers.end())
	{
		m_timers.push_back(timer);
	}
	m_timers.sort(Utility::Timer::ComparePriority);
}

}	// Namespace Engine
