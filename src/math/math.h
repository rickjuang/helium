/*************************************************************
 *	Math
 *	by Rick Juang, 2011/06/20
 *	Common math library
 *************************************************************/
#ifndef __MATH_H__
#define __MATH_H__

#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <stdlib.h>

#define EPSILON					0.000001f
#define FLOAT_MAX				3.402823466e+38F
#define PI						3.141592654f
#define PI_DOUBLE				6.283185307f
#define PI_HALF					1.570796327f
#define PI_INVERSE				0.318309886f

#define RSQRT(f)				(1.0f / Math::SqRt(f))
#define SQUARE(f)				((f) * (f))
#define RAD_TO_DEG(radian)		((radian) * (180.0f * PI_INVERSE))
#define DEG_TO_RAD(degree)		((degree) * (PI / 180.0f))

//enum { X, Y, Z, W };
//enum { R, G, B, A };
#define X						0
#define Y						1
#define Z						2
#define W						3

namespace Math
{
	inline float Abs(float f)
	{
		return abs(f);
	}

	inline int Round(float f)
	{
		return (int)((f > 0.0f) ? floor(f + 0.5f) : ceil(f - 0.5f));
	}

	inline float SqRt(float f)
	{
		return sqrt(f);
	}

	inline float Sin(float radians)
	{
		return sin(radians);
	}

	inline float Cos(float radians)
	{
		return cos(radians);
	}

	inline float Tan(float radians)
	{
		return tan(radians);
	}

	inline float ACos(float f)
	{
		return acos(f);
	}

	inline float ATan(float f)
	{
		return atan(f);
	}

	inline float ATan2(float y, float x)
	{
		return atan2(y, x);
	}

	inline int Rand(void)
	{
		return rand();
	}

	inline int Rand(int min_incl, int max_excl)
	{
		return Rand() % (max_excl - min_incl) + min_incl;
	}

	inline float Rand(float min_incl, float max_incl)
	{
		return ((max_incl - min_incl) * ((float)Rand() / (float)RAND_MAX)) + min_incl;
	}

}	// namespace Math

#endif // __MATH_H__
