/*************************************************************
 *	Physics2D
 *	by Rick Juang, 2011/09/02
 *	Simple 2D Physics
 *************************************************************/

#include <engine/globals.h>
#include <engine/game_app.h>
#include <entity/entity.h>
#include <entity/message.h>
#include <entity/component/transform.h>
#include <utility/debug.h>
//#include <graphics/TextMode.h>

#include "physics2d.h"

namespace Entity
{
Physics2D::Physics2D(void)
:	Component	()
,	m_vel		(Math::Vector3::ZERO)
,	m_acc		(Math::Vector3(0.0f, 0.0f, -0.00003f))
,	m_halfAcc	(m_acc * 0.5f)
,	m_mass		(1.0f)
{
	//SetUpdateFrequency(100, FIXED);
}

Physics2D::~Physics2D(void)
{
	//if (s_manager)
	{
		//((Physics2DManager*)s_manager)->UnregisterComponent(this);
	}
}

void Physics2D::OnInit(void)
{
	m_transform = (Transform*)GetHost()->FindComponent(CRC(0xF543030E, "Transform"));
	if (!m_transform)
	{
		m_transform = (Transform*)GetHost()->FindComponentByType(CRC(0xF543030E, "Transform"));
	}
	Dbg_Assert(m_transform != NULL);

	//SetUpdateFrequency(30.0f, FIXED);
}

void Physics2D::OnUpdate(MSec delta_time)
{
	Math::Vector3 pos = m_transform->GetWorldPosition();
	//Graphics::g_TextConsole.Set(pos, '.');

	static float bottom = -40;
	static float left = -40;
	static float right = -left;

	float dt_sq = (float)SQUARE(delta_time);
	float dt = (float)delta_time;

	pos += m_vel * dt + m_halfAcc * dt_sq;
	m_vel += m_acc * dt;

	if (pos[X] <= left)
	{
		pos[X] = left;
		m_vel[X] = -m_vel[X];
	}
	else if (pos[X] >= right)
	{
		pos[X] = right;
		m_vel[X] = -m_vel[X];
	}

	if (pos[Z] < bottom)
	{
		m_vel[Z] = -m_vel[Z];
		// manually step through oenetrating frames
		while (pos[Z] < bottom)
		{
			pos += m_vel * dt + m_halfAcc * dt_sq;
			m_vel += m_acc * dt;
		}
	}
	m_transform->SetWorldPosition(pos);
	//Graphics::g_TextConsole.Set(pos, '+');
}

}	// Namespace Entity
