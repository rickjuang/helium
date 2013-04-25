/*************************************************************
 *	Component
 *	by Rick Juang, 2011/06/12
 *	Base component class
 *************************************************************/
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <engine/defines.h>
#include <utility/container.h>
#include <utility/game_timer.h>

namespace Entity
{
	class Entity;
	class Message;
	class ComponentManager;

	class Component
	{
	public:
		Component(void);
		virtual ~Component(void);

		virtual CRC					GetType(void) = 0;
		virtual CRC					GetName(void) = 0;
	//	virtual	Component*			Clone(void) = 0;

	public:
		Entity*						GetHost(void)				{ return m_host; }
		void						Attach(Entity* host);
		void						Detach(void);
		void						Init(void)					{ OnInit(); }
		void						Kill(void);
		void						Reset(void)					{ OnReset(); }
		void						Update(MSec delta_time)		{ OnUpdate(delta_time); }
		void						Notify(Message* event)		{ OnNotify(event); }
		void						Pause(bool pause)			{ m_flags[PAUSED] = pause; pause ? OnPause() : OnResume(); }
		bool						IsPaused(void)				{ return m_flags[PAUSED]; }

	protected:
		virtual void				OnAttach(void)				{}
		virtual void				OnDetach(void)				{}
		virtual void				OnInit(void)				{}	// called after added to the host and all other components are available for referencing
		virtual void				OnReset(void)				{}
		virtual void				OnUpdate(MSec delta_time)	{}
		virtual bool				OnNotify(Message* event)	{ return false; }
		virtual void				OnPause(void)				{}
		virtual void				OnResume(void)				{}
		virtual void				OnDestroy(void)				{}

	protected:
		enum Flags
		{
			DEAD					= 0,
			PAUSED					,
			FLAG_COUNT
		};
		Entity*						m_host;
		STL::bitset<8>				m_flags;
		STL::bitset<8>				m_tags;

	private:
		static ComponentManager*	s_manager;

	};	// class Component

	//	----------------------------------------------------------
	class ComponentManager
	{
	public:
		ComponentManager(void);
		virtual ~ComponentManager(void);

		Utility::Timer*				m_timer;	// TODO: rename to Timer

		void						RegisterComponent(Component* com);
		void						UnregisterComponent(Component* com);
		bool						IsComponentRegistered(Component* com);
		bool						HasComponents(void);
		void						SetUpdateFrequency(float hertz, Utility::Timer::TimeStepType time_step_type);
		void						SetUpdateTimeLimit(MSec time);

		void						Init(void);
		void						Reset(void);
		void						Update(MSec delta_time)		{ OnUpdate(delta_time); }
		void						Notify(Message* event)		{ OnNotify(event); }
		void						Pause(bool pause);

	protected:
		virtual void				OnInit(void);
		virtual void				OnReset(void);
		virtual void				OnUpdate(MSec delta_time);
		virtual void				OnNotify(Message* event);
		virtual void				OnPause(void);
		virtual void				OnResume(void);

	protected:
		STL::list<Component*>		m_components;
	//	Utility::Timer*				m_timer;

		STL::list<Component*>::iterator m_it;
		MSec						m_roundTableTime;

	};	// class ComponentManager

//	extern ComponentManager* g_ComponentManager;

}	// namespace Entity

#endif // __COMPONENT_H__
