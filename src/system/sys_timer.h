/*************************************************************
 *	SysTimer
 *	by Rick Juang, 2011/06/15
 *	Keep track of system time since machine startup
 *************************************************************/
#ifndef __SYSTIMER_H__
#define __SYSTIMER_H__

#include <engine/units.h>

namespace System
{
	class Timer
	{
	public:
		Timer(void) : m_start(0)						{}
	//	~Timer(void)									{}

		void			Reset(void)						{ m_start = GetSystemTime(); }
		MSec			GetElapsedTime(void)			{ return GetSystemTime() - m_start; }	// elapsed time since last Reset()
		static MSec		GetSystemTime(void);

	private:
		MSec			m_start;
	};

}	// namespace System

#endif // __SYSTIMER_H__
