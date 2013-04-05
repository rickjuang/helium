/*************************************************************
 *	Entity
 *	by Rick Juang, 2011/06/21
 *	Base entity class
 *************************************************************/
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <engine/defines.h>
#include <utility/container.h>

namespace Entity
{
	class Component;
	class Message;

	// Try not to inherit from Entity. Instead, create new components when additional functionalities are needed.
	class Entity
	{
	public:
		Entity(void);
		~Entity(void);

		CRC							GetType(void)					{ return m_type; }
		CRC							GetName(void)					{ return m_name; }
	//	Entity*						Clone(void) = 0;

		void						Init(void);						//call this after all components are attached
		void						Kill(void);
		void						Reset(void);
		void						AttachComponent(Component* component);
		void						DetachComponent(Component* component);
		void						DetachComponent(CRC name);
		Component*					FindComponent(CRC name);
		Component*					FindComponent(Component* component);
		Component*					FindComponentByType(CRC type);
		void						Notify(Message* event, CRC component = 0);
		void						Pause(bool pause);
		bool						IsPaused(void)					{ return m_flags[Paused]; }

	private:
		enum Flags
		{
			Dead					= 0x01,
			Paused					= 0x02,
		};
		CRC							m_type;
		CRC							m_name;
		STL::bitset<8>				m_flags;
		STL::bitset<8>				m_tags;
		STL::map<CRC, Component*>	m_components;

	};	// class Entity

}	// namespace Entity

#endif // __ENTITY_H__
