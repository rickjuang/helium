/*************************************************************
 *	BaseAnimation
 *	by Rick Juang, 2012/01/25
 *	Base class for Animation component
 *************************************************************/

#ifndef __BASEANIMATION_H__
#define __BASEANIMATION_H__

#include <math/vector.h>
#include <utility/game_timer.h>
#include <utility/container.h>

namespace Entity
{
	class Transform;

	class BaseAnimation : public Component
	{
	public:
		BaseAnimation(void);
		virtual ~BaseAnimation(void);

		CRC							GetType(void)								{ return CRC(0x0FA3067F, "Animation"); }
		CRC							GetName(void)								{ return CRC(0x0FA3067F, "Animation"); }
	//	virtual	BaseAnimation*		Clone(void) = 0;

		virtual void				StopAnimation(void) = 0;
		virtual bool				PlayAnimation(const char* animation, MSec blend_time = 0, bool loop = false) = 0;
		virtual bool				IsPlayingAnimation(const char* animation) = 0;

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

	protected:
		static ComponentManager*	s_manager;

	};	// class BaseAnimation

}	// namespace Entity

#endif // __BASEANIMATION_H__
