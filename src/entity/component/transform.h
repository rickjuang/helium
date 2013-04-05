/*************************************************************
 *	Transform
 *	by Rick Juang, 2011/06/22
 *	Component responsible for position, orientation, size
 *************************************************************/
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <entity/component.h>
#include <math/vector.h>
#include <math/quaternion.h>
#include <utility/container.h>

#define CACHE_CHILDREN_COUNT	1

namespace Entity
{
	class Transform : public Component
	{
	public:
		Transform(void);
		virtual ~Transform(void);

		CRC							GetType(void)										{ return CRC(0xF543030E, "Transform"); }
		CRC							GetName(void)										{ return m_name; }
		void						SetName(CRC name)									{ m_name = name; }
	//	virtual	Transform*			Clone(void) = 0;

		void						SetWorldTransform(const Math::Vector3& scale, const Math::Quaternion& rot, const Math::Vector3& pos);
		Math::Vector3				GetWorldScale(void) const							{ return m_worldScale; }
		void						SetWorldScale(const Math::Vector3& scale);
		Math::Quaternion			GetWorldOrientation(void) const						{ return m_worldRot; }
		void						SetWorldOrientation(const Math::Quaternion& rot);
		Math::Vector3				GetWorldPosition(void) const						{ return m_worldPos; }
		void						SetWorldPosition(const Math::Vector3& pos);
		Math::Vector3				GetWorldDirection(void) const;
		void						SetWorldDirection(const Math::Vector3& dir, const Math::Vector3& up = Math::Vector3::UNIT_Y);

		void						SetLocalTransform(const Math::Vector3& scale, const Math::Quaternion& rot, const Math::Vector3& pos);
		Math::Vector3				GetLocalScale(void) const							{ return m_localScale; }
		void						SetLocalScale(const Math::Vector3& scale)			{ m_localScale = scale; UpdateTransform(); }
		Math::Quaternion			GetLocalOrientation(void) const						{ return m_localRot; }
		void						SetLocalOrientation(const Math::Quaternion& rot)	{ m_localRot = rot; UpdateTransform(); }
		Math::Vector3				GetLocalPosition(void) const						{ return m_localPos; }
		void						SetLocalPosition(const Math::Vector3& pos)			{ m_localPos = pos; UpdateTransform(); }
		Math::Vector3				GetLocalDirection(void) const;
		void						SetLocalDirection(const Math::Vector3& dir, const Math::Vector3& up = Math::Vector3::UNIT_Y);


		void						AddChild(Transform* child);
		void						RemoveChild(Transform* child);
		void						RemoveAllChildren(void);
		Transform*					FindChild(CRC name);
		Transform*					GetParent(void) const								{ return m_parent; }

		friend class				Transform;

	protected:
		void						UpdateTransform(void);

	protected:
		virtual void				OnAddToParent(Transform* new_parent)				{}
		virtual void				OnRemoveFromParent(Transform* ex_parent)			{}

		virtual void				OnAttach(void)										{}
		virtual void				OnInit(void)										{}
		virtual void				OnReset(void)										{}
		virtual void				OnUpdate(MSec delta_time)							{}
		virtual bool				OnNotify(Message* event);
		virtual void				OnPause(void)										{}
		virtual void				OnResume(void)										{}
		virtual void				OnDestroy(void)										{}

	protected:
		enum Flags
		{
			FLAG1 = Component::FLAG_COUNT,
			FLAG2,
			FLAG_COUNT
		};

		Math::Quaternion			m_worldRot;
		Math::Quaternion			m_localRot;
		Math::Vector3				m_worldPos;
		Math::Vector3				m_localPos;
		Math::Vector3				m_worldScale;
		Math::Vector3				m_localScale;

		CRC							m_name;
		Transform*					m_parent;
		STL::map<CRC, Transform*>	m_children;
#if CACHE_CHILDREN_COUNT
		int							m_childrenCount;
#endif

	};	// class Transform

}	// namespace Entity

#endif // __TRANSFORM_H__
