/*************************************************************
 *	Component
 *	by Rick Juang, 2011/06/21
 *	Base component class
 *************************************************************/

#include <engine/globals.h>
#include <entity/entity.h>
#include <entity/message.h>
#include <utility/debug.h>	// for Dbg_Assert

#include "component.h"

namespace Entity
{

ComponentManager* Component::s_manager = NULL;

Component::Component(void)
:	m_host			(NULL)
,	m_flags			(0)
,	m_tags			(0)
{
}

Component::~Component(void)
{
	if (m_host)
	{
		m_host->DetachComponent(this);
	}
}

void Component::Attach(Entity* host)
{
	Dbg_Assert(host != NULL);

	if (!host->FindComponent(this))
	{
		host->AttachComponent(this);
	}
	else
	{
		m_host = host;
		OnAttach();
	}
}

void Component::Detach(void)
{
	Dbg_Assert(m_host != NULL);

	if (m_host->FindComponent(this))
	{
		m_host->DetachComponent(this);
	}
	else
	{
		OnDetach();
		m_host = NULL;
	}
}

void Component::Kill(void)
{
	if (m_host)
	{
		m_host->DetachComponent(this);
	}
	m_flags[DEAD] = true;
	OnDestroy();
}

//	----------------------------------------------------------
ComponentManager::ComponentManager(void)
:	m_timer				(NULL)
,	m_it				(m_components.begin())
,	m_roundTableTime	(0)
{
}

ComponentManager::~ComponentManager(void)
{
	if (m_timer)
	{
		Global::g_GameApp->UnregisterTimer(m_timer);
		delete m_timer;
	}
}

void ComponentManager::RegisterComponent(Component* com)
{
#ifdef _DEBUG
	Dbg_Assert(IsComponentRegistered(com) == false);
#endif

	m_components.push_back(com);
}

void ComponentManager::UnregisterComponent(Component* com)
{
	//STUB
	m_components.remove(com);
}

bool ComponentManager::IsComponentRegistered(Component* com)
{
	return (STL::find(m_components.begin(), m_components.end(), com) != m_components.end());
}

bool ComponentManager::HasComponents(void)
{
	return !m_components.empty();
}

void ComponentManager::SetUpdateFrequency(float hertz, Utility::Timer::TimeStepType time_step_type)
{
	if (m_timer && m_timer->GetType() != time_step_type)
	{
		delete m_timer;
		m_timer = NULL;
	}

	if (!m_timer)
	{
		switch (time_step_type)
		{
		case Utility::Timer::FIXED:
			m_timer = new Utility::FixedStepTimer();
			break;
		case Utility::Timer::VARIABLE:
			m_timer = new Utility::Timer();
			break;
		default:
			Dbg_Assert(false, "TimeStepType unsupported!");
			m_timer = new Utility::Timer();
		}
	}

	m_timer->SetFrequency(hertz);
	m_timer->SetUpdater(this);

	Reset();
}

void ComponentManager::SetUpdateTimeLimit(MSec budget)
{
	m_roundTableTime = budget;
}

void ComponentManager::Init(void)
{
	OnInit();
	/* no longer needed - timers are now self-registering.
	if (m_timer)
	{
		Global::g_GameApp->RegisterTimer(m_timer);
	}
	*/
}

void ComponentManager::Reset(void)
{
	m_timer->Reset();
	m_timer->Start();
	OnReset();
}

void ComponentManager::Pause(bool pause)
{
	if (pause)
	{
		m_timer->Stop();
		OnPause();
	}
	else
	{
		m_timer->Start();
		OnResume();
	}
}

void ComponentManager::OnInit(void)
{
//	g_ComponentManager = this;
}

void ComponentManager::OnReset(void)
{
	for (STL::list<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->Reset();
	}
	m_it = m_components.begin();
}

void ComponentManager::OnUpdate(MSec delta_time)
{
	// implement round table update here if necessary

	if (m_roundTableTime > 0)
	{
		System::Timer timer;
		timer.Reset();
		while (m_it != m_components.end())
		{
			(*m_it)->Update(delta_time);
			if (timer.GetElapsedTime() < m_roundTableTime)
			{
				++m_it;
			}
			else
			{
				m_it = m_components.begin();
				break;
			}
		}
		if (m_it == m_components.end())
		{
			m_it = m_components.begin();
		}
	}
	else
	{
		for (STL::list<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->Update(delta_time);
		}
	}
}

void ComponentManager::OnNotify(Message* event)
{
	switch (event->Type)
	{
	case 0:
		break;
	default:
		break;
	}
}

void ComponentManager::OnPause(void)
{
	for (STL::list<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->Pause(true);
	}
}

void ComponentManager::OnResume(void)
{
	for (STL::list<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->Pause(false);
	}
}

}	// Namespace Entity
