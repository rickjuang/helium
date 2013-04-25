/*************************************************************
 *	BaseAnimation
 *	by Rick Juang, 2012/01/25
 *	Base class for Animation component
 *************************************************************/

#include <engine/globals.h>
#include <engine/game_app.h>
#include <entity/entity.h>
#include <entity/message.h>
#include <entity/component/transform.h>
#include <utility/debug.h>
//#include <graphics/TextMode.h>

#include "base_animation.h"

namespace Entity
{

ComponentManager* BaseAnimation::s_manager = NULL;

BaseAnimation::BaseAnimation(void)
:	Component	()
{
	if (!s_manager)
	{
		s_manager = new ComponentManager();
		s_manager->SetUpdateFrequency(60, Utility::Timer::FIXED);
	}
	s_manager->RegisterComponent(this);
}

BaseAnimation::~BaseAnimation(void)
{
	if (s_manager)
	{
		s_manager->UnregisterComponent(this);
		if (!s_manager->HasComponents())
		{
			delete s_manager;
			s_manager = NULL;
		}
	}
}

void BaseAnimation::OnInit(void)
{
	m_transform = (Transform*)GetHost()->FindComponent(CRC(0xF543030E, "Transform"));
	if (!m_transform)
	{
		m_transform = (Transform*)GetHost()->FindComponentByType(CRC(0xF543030E, "Transform"));
	}
	Dbg_Assert(m_transform != NULL);
}

void BaseAnimation::OnUpdate(MSec delta_time)
{

}

}	// Namespace Entity
