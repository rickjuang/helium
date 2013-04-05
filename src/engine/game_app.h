/*************************************************************
 *	GameApp
 *	by Rick Juang, 2011/06/20
 *	Base Game class
 *************************************************************/
#ifndef __GAMEAPP_H__
#define __GAMEAPP_H__

#include <engine/globals.h>
#include <system/sys_timer.h>
#include <utility/game_timer.h>
#include <utility/container.h>

namespace Graphics
{
	class BaseRenderer;
}

namespace Engine
{
	class GameApp
	{
	public:
		enum AppState
		{
			INIT = 0,
			RUNNING,
			QUIT,
			FLAG_COUNT
		};

		GameApp(void);
		static int					Main(GameApp* game, int argc = 0, char* argv[] = NULL);

	public:

		void						Init(void);
		void						Start(void);
		void						Update(void);
		void						Exit(void);
		MSec						GetAppTime(void)						{ return m_sysTimer.GetElapsedTime(); }
		AppState					GetAppState(void)						{ return m_state; }
		void						RegisterTimer(Utility::Timer* timer);
		void						UnregisterTimer(Utility::Timer* timer)	{ m_timers.remove(timer); }
		void						SetAppState(AppState state)				{ m_state = state; }

	protected:
		virtual void				OnInit(void)							{}
		virtual void				OnStart(void)							{}
		virtual void				OnUpdate(void)							{}
		virtual void				OnExit(void)							{}

		Graphics::BaseRenderer*		m_renderer;

	private:
		System::Timer				m_sysTimer;
		STL::list<Utility::Timer*>	m_timers;
		AppState					m_state;
	};

}	// namespace Engine

namespace Global
{
	//extern Engine::GameApp*	g_GameApp;

}	// Namespace Global

#endif // __GAMEAPP_H__
