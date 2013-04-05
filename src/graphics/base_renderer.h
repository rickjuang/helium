/*************************************************************
 *	BaseRenderer
 *	by Rick Juang, 2011/12/19
 *	Base renderer class
 *************************************************************/
#ifndef __BASERENDERER_H__
#define __BASERENDERER_H__

#include <engine/globals.h>
#include <system/sys_timer.h>
#include <utility/game_timer.h>

namespace Graphics
{
	class BaseRenderer
	{
	public:
		BaseRenderer(void);
		virtual ~BaseRenderer(void);

		void						Init(float fps = 60.0f);
		void						Start(void);
		void						Render(void);
		void						Shutdown(void);

	protected:
		virtual void				OnInit(void)							{}
		virtual void				OnStart(void)							{}
		virtual void				OnRender(void)							{}
		virtual void				OnShutdown(void)						{}

		Utility::Timer				m_renderTimer;

	private:
		static void					DoRender(MSec delta_time);
		static BaseRenderer*		s_instance;
	};

}	// namespace Game

#endif // __BASERENDERER_H__
