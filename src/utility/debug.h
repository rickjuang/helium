/*************************************************************
 *	Debug
 *	by Rick Juang, 2011/06/18
 *	Debugging utilities
 *************************************************************/
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <engine/defines.h>

#define DEBUG	_DEBUG

#if DEBUG
	void Dbg_Assert(int expression);
	void Dbg_Assert(int expression, const char* format, ...);
	void Dbg_PrintF(const char* format, ...);
#else
	#define Dbg_Assert(...)
	#define Dbg_PrintF(...)
#endif

#endif // __DEBUG_H__
