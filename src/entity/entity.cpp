/*************************************************************
 *	Entity
 *	by Rick Juang, 2011/06/21
 *	Base entity class
 *************************************************************/

#include <utility/debug.h>
#include <entity/component.h>

#include "entity.h"

namespace Entity
{

Entity::Entity(void)
:	m_type			(0)
,	m_name			(0)
,	m_flags			(0)
,	m_tags			(0)
{
}

Entity::~Entity(void)
{
	//if you need to do anything that might invoke virtual function calls, do it in Kill()
}

void Entity::Init(void)
{
	for (STL::map<CRC, Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->Init();
	}
}

void Entity::Kill(void)
{
	m_flags[Dead] = true;
	for (STL::map<CRC, Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->Kill();
		delete it->second;
	}
	m_components.clear();
}

void Entity::Reset(void)
{
	for (STL::map<CRC, Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->Reset();
	}
}

void Entity::AttachComponent(Component* component)
{
	Dbg_Assert(component != NULL);
	Dbg_Assert(FindComponent(component) == NULL);

	m_components.insert(STL::pair<CRC, Component*>(component->GetName(), component));
	component->Attach(this);
}

void Entity::DetachComponent(Component* component)
{
	Dbg_Assert(component != NULL);

	m_components.erase(component->GetName());
	component->Detach();
}

void Entity::DetachComponent(CRC name)
{
	Component* component = FindComponent(name);
	if (component)
	{
		DetachComponent(component);
	}
}

Component* Entity::FindComponent(CRC name)
{
	STL::map<CRC, Component*>::iterator it = m_components.find(name);
	if (it != m_components.end())
	{
		return it->second;
	}
	return NULL;
}

Component* Entity::FindComponent(Component* component)
{
	STL::map<CRC, Component*>::reverse_iterator it;
	for (it = m_components.rbegin() ; it != m_components.rend(); ++it)
	{
		if (it->second == component)
		{
			return it->second;
		}
	}
	return NULL;
}

Component* Entity::FindComponentByType(CRC type)
{
	STL::map<CRC, Component*>::reverse_iterator it;
	for (it = m_components.rbegin() ; it != m_components.rend(); ++it)
	{
		if (it->second->GetType() == type)
		{
			return it->second;
		}
	}
	return NULL;
}

void Entity::Notify(Message* event, CRC component)
{
	if (component)
	{
		STL::map<CRC, Component*>::iterator it = m_components.find(component);
		if (it != m_components.end())
		{
			it->second->Notify(event);
		}
	}
	else
	{
		for (STL::map<CRC, Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			it->second->Notify(event);
		}
	}
}

void Entity::Pause(bool pause)
{
	m_flags[Paused] = pause;
	for (STL::map<CRC, Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->Pause(pause);
	}
}

}	// Namespace Entity
