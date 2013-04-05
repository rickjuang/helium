/*************************************************************
 *	GameTimer
 *	by Rick Juang, 2011/06/15
 *	In-game world clock/timer
 *************************************************************/
#ifndef __GAMETIMER_H__
#define __GAMETIMER_H__

#include <engine/defines.h>
#include <engine/units.h>

namespace Entity
{
	class ComponentManager;
}
typedef Entity::ComponentManager*	Updater;
typedef	void (*TimerCallBack)		(MSec delta_time);

namespace Utility
{
	class Timer
	{
	public:
		enum TimeStepType
		{
			VARIABLE	= CRC(0x2CE33943, "Default"),		// for everything else
			FIXED		= CRC(0xADE68A85, "FixedStep"),		// mainly for physics
		};

		Timer(void);
		virtual ~Timer(void);

		virtual CRC		GetType(void)						{ return VARIABLE; }
		virtual void	Update(void);
		virtual void	Reset(bool update_first = true);
		virtual void	Start(void)							{ m_running = true; }
		virtual void	Stop(void)							{ m_running = false; }
		virtual bool	IsRunning(void) const				{ return m_running; }
		virtual void	SetFrequency(float hertz);
		virtual float	GetFrequency(void) const			{ return 1000.0f / (float)Period; }
		virtual void	SetHandler(TimerCallBack handler)	{ m_tickHandler = handler; }
		virtual void	SetUpdater(Updater updater)			{ m_updater = updater; }

		virtual MSec	GetTime(void) const					{ return m_runningTime; }

		MSec			Period;
		float			TimeScale;

		static bool		ComparePriority(const Utility::Timer* lhs, const Utility::Timer* rhs)
		{
			return (lhs->Period < rhs->Period);
		}

	protected:
		MSec			m_runningTime;
		MSec			m_lastTime;
		MSec			m_accumTime;
		MSec			m_elapsedTime;
		bool			m_running;

		TimerCallBack	m_tickHandler;
		Updater			m_updater;

	};	// class Timer

	//	----------------------------------------------------------
	class FixedStepTimer : public Timer
	{
	public:
		FixedStepTimer(void);
		virtual ~FixedStepTimer(void);

		CRC				GetType(void)						{ return FIXED; }
		void			Update(void);

		MSec			MaxFrameTime;

	};	// class FixedStepTimer

}	// namespace Utility

#endif // __GAMETIMER_H__
