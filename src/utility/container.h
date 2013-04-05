/*************************************************************
 *	Container
 *	by Rick Juang, 2011/09/07
 *	STL container utilities
 *************************************************************/
#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#define EASTL	0

#if EASTL
	#define STL	eastl
	#include <EASTL/bitset.h>
	#include <EASTL/map.h>
	#include <EASTL/list.h>
	#include <EASTL/vector.h>
	#include <EASTL/algorithm.h>
//	#include <EASTL/hash_map.h>
//	#include <EASTL/string.h>
#else
	#define STL	std
	#include <bitset>
	#include <map>
	#include <list>
	#include <vector>
	#include <algorithm>
//	#include <unordered_map>
//	#include <hash_map>
//	#include <string>
#endif

#if EASTL
// EASTL expects us to define these, see allocator.h line 194
void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line);
void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line);

// EASTL also wants us to define this (see string.h line 197)
int Vsnprintf8(char8_t* pDestination, size_t n, const char8_t* pFormat, va_list arguments);
#endif

#endif // __CONTAINER_H__
