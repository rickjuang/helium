/*************************************************************
 *	EventLoop
 *	by Rick Juang, 2011/06/20
 *	OS dependent event loop
 *************************************************************/
#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

namespace Engine
{
	class GameApp;
}

namespace System
{
	void	EventLoop(Engine::GameApp* gameApp);

}	// namespace System

#endif // __EVENTLOOP_H__
