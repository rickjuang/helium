/*************************************************************
 *	ANSIConsole
 *	by Rick Juang, 2013/04/03
 *	Console utility for ANSI-compliant terminal
 *************************************************************/
#ifndef __ANSICONSOLE_H__
#define __ANSICONSOLE_H__

namespace Utility
{
	class ANSIConsole
	{
	public:
		ANSIConsole(void);

		void	Clear(void);
		void	Write(int x, int y, const char* str, bool clip = true);
		void	PrintF(int x, int y, const char* format, ...);

		void	UpdateSize(void);
		void	SetRegPt(int x, int y);
		void	CenterRegPt(bool b);

	private:
		int		m_width;
		int		m_height;
		int		m_regPtX;
		int		m_regPtY;
	};
}	// namespace Utility

#endif // __ANSICONSOLE_H__
