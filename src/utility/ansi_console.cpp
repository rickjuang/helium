/*************************************************************
 *	ANSIConsole
 *	by Rick Juang, 2013/04/03
 *	Console utility for ANSI-compliant terminal
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdarg.h>		// for va_list

#include "ansi_console.h"

namespace Utility
{

ANSIConsole::ANSIConsole(void)
{
	CenterRegPt(true);
}

void ANSIConsole::Clear(void)
{
	printf("\E[H\E[2J\n");
}

void ANSIConsole::Write(int x, int y, const char* str, bool clip)
{
	int col = x + m_regPtX;
	int row = y + m_regPtY;
	char* head = (char*)str;

	if (clip)
	{
		char buf[256];
		strncpy(buf, str, sizeof(buf));
		head = buf;

		//clip top
		if (*head != '\0' && row < 1)
		{
			*head = '\0';
			row = 1;
		}
		//clip bottom
		if (*head != '\0' && row > m_height)
		{
			*head = '\0';
			row = m_height;
		}
		//clip left
		if (*head != '\0' && col < 1)
		{
			head -= col;
			col = 1;
		}
		//clip right
		if (*head != '\0' && col + (int)strlen(head) > m_width)
		{
			int idx = m_width - col + 1;
			head[(idx < 0) ? 0 : idx] = '\0';
		}
	}

	if (*head != '\0')
	{
		printf("\E[%d;%dH%s\E[%d;1H\n", row, col, head, m_height - 1);
	}
}

void ANSIConsole::PrintF(int x, int y, const char* format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
#ifdef _WIN32
	vsprintf_s(buffer, sizeof(buffer), format, args);
#else
	vsprintf(buffer, format, args);
#endif
	va_end(args);

	char* line = buffer;
	int len = strlen(buffer);
	for (int i = 0; i <= len; i++)
	{
		if (buffer[i] == '\n')
		{
			buffer[i] = '\0';
			Write(x, y++, line);
			line = buffer + i + 1;
		}
		else if (buffer[i] == '\0')
		{
			Write(x, y, line);
		}
	}
}

void ANSIConsole::UpdateSize(void)
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	m_width = w.ws_col;
	m_height = w.ws_row;
}

void ANSIConsole::SetRegPt(int x, int y)
{
	m_regPtX = x;
	m_regPtY = y;
}

void ANSIConsole::CenterRegPt(bool b)
{
	UpdateSize();
	if (b)
	{
		m_regPtX = m_width / 2 + 1;
		m_regPtY = m_height / 2 + 1;
	}
	else
	{
		m_regPtX = 1;
		m_regPtY = 1;
	}
}

}	// namespace Utility


//	----------------------------------------------------------
//	Test program for ANSIConsole
#if 0
#if __cplusplus > 199711L
#include <chrono>
#include <thread>
#endif
#define _USE_MATH_DEFINES
#include <math.h>

inline float Deg2Rad(float degree)
{
	return degree * (M_PI / 180.0f);
}

inline int Round(float f)
{
	return (int)((f > 0.0f) ? floor(f + 0.5f) : ceil(f - 0.5f));
}

inline void Sleep(int msec)
{
#if __cplusplus > 199711L
	std::this_thread::sleep_for(std::chrono::milliseconds(msec));
#else
	usleep(msec * 1000);
#endif
}

int main(int argc, char *argv[])
{
	Utility::ANSIConsole console;
	console.Clear();

	float font_aspect = 0.45f;
	float r = 16.0f;
	for (int i = 0; i < 65535; i++)
	{
		//console.Clear();

		float rad = Deg2Rad(i);
		float x = r * cos(rad);
		float y = r * sin(rad);

		//console.Write(Round(x), Round( y * font_aspect), ".");
		//console.Write(Round(y), Round(-x * font_aspect), " ");
		console.PrintF(Round(x), Round( y * font_aspect), "+ - +\n+%s+\n+ - +", "ABC");
		console.PrintF(Round(y), Round(-x * font_aspect), "     \n     \n     ");

		Sleep(15);

	}

	return 0;
}
#endif
