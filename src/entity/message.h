/*************************************************************
 *	Component
 *	by Rick Juang, 2011/06/22
 *	Base message class
 *************************************************************/
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <engine/defines.h>

namespace Entity
{
	class Entity;

	class Message
	{
	public:
		Message(void)
		:	Type	(0)
		,	Data	(NULL)
		{}
		Message(CRC type, void* data = NULL)
		:	Type	(type)
		,	Data	(data)
		{}

		CRC		Type;
		void*	Data;
	};	// class Message

}	// namespace Entity

#endif // __MESSAGE_H__
