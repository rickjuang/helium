/*************************************************************
 *	Debug
 *	by Rick Juang, 2011/06/18
  *************************************************************/

#include <engine/defines.h>

#include "debug.h"
//#if DEBUG
#include <stdio.h>		// for vsprintf_s()
#include <stdarg.h>		// for va_list
#include <string.h>		// for memset()
//#include <conio.h>		// for clrscr()
#include <assert.h>		// for assert()
//#endif

#ifdef _WIN32
#include <windows.h>	// for SetConsoleCursorPosition()
#endif //USE_WIN_CONSOLE

#ifdef _WIN32
HANDLE gStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
#endif //_WIN32

inline void clear_console(void)
{
#ifdef _WIN32
	static COORD pos = { 0, 0 };

	#if 1
	CONSOLE_SCREEN_BUFFER_INFO scr_buf_info;

	// Get the number of character cells in the current buffer
	if (!GetConsoleScreenBufferInfo(gStdOut, &scr_buf_info))
	{
		return;
	}

	DWORD con_size = scr_buf_info.dwSize.X * scr_buf_info.dwSize.Y;
	DWORD ch;

	// Fill the entire screen with blanks
	if (!FillConsoleOutputCharacter(gStdOut, (WCHAR)' ', con_size, pos, &ch))
	{
		return;
	}

	// Get the current text attribute.
	if (!GetConsoleScreenBufferInfo(gStdOut, &scr_buf_info))
	{
		return;
	}

	// Set the buffer's attributes accordingly.
	if (!FillConsoleOutputAttribute(gStdOut, scr_buf_info.wAttributes, con_size, pos, &ch))
	{
		return;
	}
	#endif
	// Put the cursor at its home coordinates.
	SetConsoleCursorPosition(gStdOut, pos);

#else
	system("clear");
#endif //_WIN32
}

inline void write_console(char* output, int size)
{
#if 0
//#ifdef _WIN32
	DWORD ch;
	WriteConsole(gStdOut, output, size, &ch, NULL);
#else
	printf(output);
#endif //_WIN32
}

//	----------------------------------------------------------
#if DEBUG
void Dbg_Assert(int expression)
{
	assert(expression);
}

void Dbg_Assert(int expression, const char* format, ...)
{
	if (expression) return;

	assert(expression);

	char buffer[256];
	va_list args;
	va_start(args, format);
#ifdef _WIN32
	vsprintf_s(buffer, sizeof(buffer), format, args);
#else
	vsprintf(buffer, format, args);
#endif
	printf(buffer);
	va_end(args);
}

void Dbg_PrintF(const char* format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
#ifdef _WIN32
	vsprintf_s(buffer, sizeof(buffer), format, args);
#else
	vsprintf(buffer, format, args);
#endif
	printf(buffer);
	va_end(args);
}

#endif	//DEBUG
