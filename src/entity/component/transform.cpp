/*************************************************************
 *	Transform
 *	by Rick Juang, 2011/06/22
 *	Component responsible for position, orientation, size
 *************************************************************/

#include <entity/entity.h>
#include <entity/message.h>
#include <entity/component.h>
#include <utility/debug.h>

#include "transform.h"

namespace Entity
{

Transform::Transform(void)
:	Component		()
,	m_worldRot		(Math::Quaternion::IDENTITY)
,	m_localRot		(Math::Quaternion::IDENTITY)
,	m_worldPos		(Math::Vector3::ZERO)
,	m_localPos		(Math::Vector3::ZERO)
,	m_worldScale	(Math::Vector3::ONE)
,	m_localScale	(Math::Vector3::ONE)
,	m_name			(CRC(0xF543030E, "Transform"))
,	m_parent		(NULL)
#if CACHE_CHILDREN_COUNT
,	m_childrenCount	(0)
#endif
{
}

Transform::~Transform(void)
{
	RemoveAllChildren();
	if (m_parent)
	{
		m_parent->RemoveChild(this);
	}
}

bool Transform::OnNotify(Message* event)
{
	bool handled = false;
	switch (event->Type)
	{
	case CRC(0x97608351, "UpdateTransform"):
		{
			//update children
			#if CACHE_CHILDREN_COUNT
			if (m_childrenCount)
			#endif
			{
				for (STL::map<CRC, Transform*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
				{
					it->second->UpdateTransform();
				}
			}
			handled = true;
		}
		break;
	default:
		break;
	}

	return handled;
}

void Transform::SetWorldTransform(const Math::Vector3& scale, const Math::Quaternion& rot, const Math::Vector3& pos)
{
	m_worldScale = scale;
	m_worldRot = rot;
	m_worldPos = pos;

	//update local transform
	if (m_parent)
	{
		Math::Quaternion inv_parent_world_rot = m_parent->GetWorldOrientation().GetInverse();
		Math::Vector3 inv_parent_world_scale = 1.0f / m_parent->GetWorldScale();

		m_localScale = scale.GetScaled(inv_parent_world_scale);
		m_localRot = rot * inv_parent_world_rot;
		m_localPos = inv_parent_world_rot * (pos - m_parent->GetWorldPosition()).GetScaled(inv_parent_world_scale);
	}

	UpdateTransform();
}

void Transform::SetWorldScale(const Math::Vector3& scale)
{
	m_worldScale = scale;

	if (m_parent)
	{
		Math::Vector3 inv_parent_world_scale = 1.0f / m_parent->GetWorldScale();
		m_localScale = scale.GetScaled(inv_parent_world_scale);
	}
	else
	{
		m_localScale = scale;
	}

	UpdateTransform();
}

void Transform::SetWorldOrientation(const Math::Quaternion& rot)
{
	m_worldRot = rot;

	if (m_parent)
	{
		m_localRot = rot * m_parent->GetWorldOrientation().GetInverse();
	}
	else
	{
		m_localRot = rot;
	}

	UpdateTransform();
}

void Transform::SetWorldPosition(const Math::Vector3& pos)
{
	m_worldPos = pos;

	if (m_parent)
	{
		Math::Vector3 inv_parent_world_scale = 1.0f / m_parent->GetWorldScale();
		m_localPos = m_parent->GetWorldOrientation().GetInverse() * (pos - m_parent->GetWorldPosition()).GetScaled(inv_parent_world_scale);
	}
	else
	{
		m_localPos = pos;
	}

	UpdateTransform();
}

Math::Vector3 Transform::GetWorldDirection(void) const
{
	Math::Vector3 right, up, forward;
	m_worldRot.GetAxes(right, up, forward);

	return forward;
}

void Transform::SetWorldDirection(const Math::Vector3& dir, const Math::Vector3& up)
{
	Math::Vector3 right = dir.Cross(up);
	m_worldRot.Set(right, up, dir);

	if (m_parent)
	{
		m_localRot = m_worldRot * m_parent->GetWorldOrientation().GetInverse();
	}
	else
	{
		m_localRot = m_worldRot;
	}

	UpdateTransform();
}

Math::Vector3 Transform::GetLocalDirection(void) const
{
	Math::Vector3 right, up, forward;
	m_localRot.GetAxes(right, up, forward);

	return forward;
}

void Transform::SetLocalDirection(const Math::Vector3& dir, const Math::Vector3& up)
{
	Math::Vector3 right = dir.Cross(up);
	m_localRot.Set(right, up, dir);

	UpdateTransform();
}

void Transform::UpdateTransform(void)
{
	//update world transform
	if (m_parent)
	{
		Math::Quaternion parent_world_rot = m_parent->GetWorldOrientation();
		Math::Vector3 parent_world_scale = m_parent->GetWorldScale();

		m_worldScale = m_localScale;
		m_worldScale.Scale(parent_world_scale);

		//TODO: double check with the quaternion multification order
		//m_worldRot = parent_world_rot * m_localRot; OR
		m_worldRot = m_localRot;
		m_worldRot *= parent_world_rot;

		m_worldPos = m_localPos;
		m_worldPos.Scale(parent_world_scale);
		m_worldPos.Rotate(parent_world_rot);
		m_worldPos += m_parent->GetWorldPosition();
	}
	else
	{
		m_worldScale = m_localScale;
		m_worldRot = m_localRot;
		m_worldPos = m_localPos;
	}

	Message msg(CRC(0x97608351, "UpdateTransform"));
	Notify(&msg);
}

void Transform::AddChild(Transform* child)
{
	Dbg_Assert(child != NULL);
	Dbg_Assert(child->m_parent == NULL);
	if (child && child->m_parent == NULL)
	{
		m_children.insert(STL::pair<CRC, Transform*>(child->GetName(), child));
		#if CACHE_CHILDREN_COUNT
		m_childrenCount++;
		#endif
		child->m_parent = this;
		child->OnAddToParent(m_parent);

		//force-update local transform
		child->SetWorldTransform(child->GetWorldScale(), child->GetWorldOrientation(), child->GetWorldPosition());
	}
}

void Transform::RemoveChild(Transform* child)
{
	Dbg_Assert(child != NULL);
	Dbg_Assert(child->m_parent == this);
	if (child && child->m_parent == this)
	{
		m_children.erase(child->GetName());
		#if CACHE_CHILDREN_COUNT
		m_childrenCount--;
		#endif
		child->m_parent = NULL;
		child->OnRemoveFromParent(this);
	}
}

void Transform::RemoveAllChildren(void)
{
	#if CACHE_CHILDREN_COUNT
	if (m_childrenCount)
	#endif
	{
		#if CACHE_CHILDREN_COUNT
		m_childrenCount = 0;
		#endif
		for (STL::map<CRC, Transform*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			it->second->m_parent = NULL;
			it->second->OnRemoveFromParent(this);
		}
		m_children.clear();
	}
}

Transform* Transform::FindChild(CRC name)
{
	STL::map<CRC, Transform*>::iterator it = m_children.find(name);
	if (it != m_children.end())
	{
		return it->second;
	}
	return NULL;
}

}	// Namespace Entity
