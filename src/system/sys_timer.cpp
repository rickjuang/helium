/*************************************************************
 *	SysTimer
 *	by Rick Juang, 2011/06/15
 *	Keep track of system time since machine startup
 *************************************************************/

#include <engine/defines.h>

#include <system/sys_timer.h>

#if __cplusplus > 199711L
	#include <chrono>
#else
	#ifdef _WIN32
		#define WIN32_LEAN_AND_MEAN
		#include <Windows.h>
		#include <MMSystem.h>
	#elif __linux__
		#include <time.h>
	#endif
	#define CHECK_TIME_WRAP		1
#endif


namespace System
{

MSec Timer::GetSystemTime(void)
{
#if __cplusplus > 199711L
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	auto duration = now.time_since_epoch();
	auto msecs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return (MSec)msecs;
#else
	#ifdef _WIN32
	LARGE_INTEGER frequency;

	if (QueryPerformanceFrequency(&frequency))
	{
		LARGE_INTEGER counter;
		static LARGE_INTEGER cur_time = { 0 };

		QueryPerformanceCounter(&counter);

		#if CHECK_TIME_WRAP
		if (counter.LowPart < cur_time.LowPart)
		{
			cur_time.HighPart++;
		}
		#endif	// CHECK_TIME_WRAP

		cur_time.LowPart = counter.LowPart;

		return (MSec)(cur_time.QuadPart * 1000 / frequency.QuadPart);
	}
	else
	{
		// bummer, hardware does not support high-resolution counter. the next best thing is timeGetTime
		return timeGetTime();
	}
	#elif __linux__
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (MSec)(now.tv_sec * 1000000 + now.tv_nsec / 1000);
	#endif
#endif
}

}	// Namespace System
