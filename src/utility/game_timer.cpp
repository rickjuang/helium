/*************************************************************
 *	SysTimer
 *	by Rick Juang, 2011/06/15
 *	FixedStepTimer is based on one of Glenn Fiedler's implementation
 *	http://gafferongames.com/game-physics/fix-your-timestep/
 *************************************************************/

#include <engine/defines.h>
#include <engine/globals.h>
#include <engine/game_app.h>
#include <system/sys_timer.h>
#include <entity/component.h>

#include "game_timer.h"
#include "debug.h"

#define DEFAULT_MAX_FRAME_TIME		100
#define DEFAULT_PERIOD				17	// roughly 59 fps

namespace Utility
{

Timer::Timer(void)
:	Period			(DEFAULT_PERIOD)
,	TimeScale		(1.0f)
,	m_running		(false)
,	m_runningTime	(0)
,	m_lastTime		(0)
,	m_accumTime		(Period)
,	m_tickHandler	(NULL)
,	m_updater		(NULL)
{
	if (Global::g_GameApp)
	{
		Global::g_GameApp->RegisterTimer(this);
	}
}

Timer::~Timer(void)
{
	if (Global::g_GameApp)
	{
		Global::g_GameApp->UnregisterTimer(this);
	}
}

void Timer::Reset(bool update_first)
{
	m_lastTime = System::Timer::GetSystemTime();
	m_runningTime = m_accumTime = 0;
	m_accumTime = update_first ? Period : 0;	// set to Period to start update right away, set to 0 to wait for 1 tick to start
}

void Timer::Update(void)
{
	MSec cur_time = System::Timer::GetSystemTime();
	MSec elapsed_time = m_running ? (MSec)(TimeScale * (cur_time - m_lastTime)) : 0;

	m_lastTime = cur_time;
	m_accumTime += elapsed_time;
	m_runningTime += elapsed_time;

	if (m_accumTime >= Period)
	{
		if (m_tickHandler)
		{
			(*m_tickHandler)(m_accumTime);
		}
		if (m_updater)
		{
			m_updater->Update(m_accumTime);
		}

		m_accumTime = 0;
	}
}

void Timer::SetFrequency(float hertz)
{
	Period = (MSec)(1000.0f / hertz);
	if (Global::g_GameApp)
	{
		Global::g_GameApp->RegisterTimer(this);
	}
}

//	----------------------------------------------------------
FixedStepTimer::FixedStepTimer(void)
:	Timer			()
,	MaxFrameTime	(DEFAULT_MAX_FRAME_TIME)
{
}

FixedStepTimer::~FixedStepTimer(void)
{
}

void FixedStepTimer::Update(void)
{
	//static float m_next_state, m_last_state;

	MSec cur_time = System::Timer::GetSystemTime();
	MSec elapsed_time = m_running ? cur_time - m_lastTime : 0;

	//cap elapsed_time
	if (elapsed_time > MaxFrameTime)
	{
		elapsed_time = MaxFrameTime;
	}

	elapsed_time = (MSec)(elapsed_time * TimeScale);

	m_lastTime = cur_time;
	m_accumTime += elapsed_time;
	m_runningTime += elapsed_time;

	while (m_accumTime >= Period)
	{
		//m_last_state = m_next_state;

		if (m_tickHandler)
		{
			(*m_tickHandler)(Period);
		}
		if (m_updater)
		{
			m_updater->Update(Period);
		}

		m_accumTime -= Period;
	}

	//const float alpha = (float)m_accumTime / (float)Period;
	//float cur_state = m_next_state * alpha + m_last_state * (1.0f - alpha);
	//if (cur_state){}
}

}	// Namespace Utility

