/*************************************************************
 *	Physics2D
 *	by Rick Juang, 2011/09/02
 *	Simple 2D Physics
 *************************************************************/
#ifndef __PHYSICS2D_H__
#define __PHYSICS2D_H__

#include <math/vector.h>
#include <utility/game_timer.h>
#include <utility/container.h>

namespace Entity
{
	class Transform;

	class Physics2D : public Component
	{
	public:
		Physics2D(void);
		virtual ~Physics2D(void);

		CRC							GetType(void)								{ return CRC(0x178CDE7F, "Physics"); }
		CRC							GetName(void)								{ return CRC(0x178CDE7F, "Physics"); }
	//	virtual	Physics2D*			Clone(void) = 0;

		void						SetAcceleration(const Math::Vector3& acc)	{ m_acc = acc; m_halfAcc = acc * 0.5f; }
		Math::Vector3				GetAcceleration(void)	const				{ return m_acc; }
		void						SetVelocity(const Math::Vector3& vel)		{ m_vel = vel; }
		Math::Vector3				GetVelocity(void)	const					{ return m_vel; }
		void						SetMass(float mass)							{ m_mass = mass; }
		float						GetMass(void) const							{ return m_mass; }
	//	void						ApplyForce(const Math::Vector3& force)		{ m_acc += force / m_mass; }//WRONG

	protected:
		virtual void				OnAttach(void)								{}
		virtual void				OnInit(void);
		virtual void				OnReset(void)								{}
		virtual void				OnUpdate(MSec delta_time);
		virtual bool				OnNotify(Message* event)					{ return false; }
		virtual void				OnPause(void)								{}
		virtual void				OnResume(void)								{}
		virtual void				OnDestroy(void)								{}

	protected:
		enum Flags
		{
			FLAG1 = Component::FLAG_COUNT,
			FLAG2,
			FLAG_COUNT
		};

		Transform*					m_transform;

		Math::Vector3				m_vel;
		Math::Vector3				m_acc;
		Math::Vector3				m_halfAcc;
		float						m_mass;

	};	// class Physics2D

}	// namespace Entity

#endif // __PHYSICS2D_H__
