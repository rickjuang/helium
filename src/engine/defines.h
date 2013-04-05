/*************************************************************
 *	Defines
 *	by Rick Juang, 2011/06/20
 *	Common definitions
 *************************************************************/
#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <stdlib.h>
#include <stddef.h>				// for NULL

#include <engine/pragmas.h>
#include <engine/units.h>

#ifndef NULL
#ifdef __cplusplus
#define NULL					0
#else
#define NULL					((void*)0)
#endif
#endif	// NULL
#define CRC(crc, comment)		((CRC)crc)

typedef unsigned int			CRC;

#endif // __DEFINES_H__
