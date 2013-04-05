/*************************************************************
 *	Quaternion
 *	by Rick Juang, 2011/08/12
 *	Quaternion math library
 *************************************************************/
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include <math/math.h>
//#include <utility/debug.h>	// for Dbg_Assert

namespace Math
{
	class Vector3;

	class Quaternion
	{
	public:
		Quaternion(void) {}															// Do nothing for speedy creation
		Quaternion(const Quaternion& src);
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Vector3& axis, float radians)								{ Set(axis, radians); }
		Quaternion(const Vector3& orig, const Vector3& dest)						{ Set(orig, dest); }
		Quaternion(const Vector3& right, const Vector3& up, const Vector3& forward)	{ Set(right, up, forward); }
		//Quaternion(const Matrix3& mat);

		void					Set(float x, float y, float z, float w);
		void					Set(const Vector3& axis, float radians);
		void					Set(const Vector3& orig, const Vector3& dest);
		void					Set(const Vector3& right, const Vector3& up, const Vector3& forward);

		void					Normalize(void);
		void					Conjugate(void);
		void					Invert(void);

		float					GetNorm(void) const;
		Quaternion				GetConjugate(void) const;
		Quaternion				GetInverse(void) const;
		void					GetAxes(Vector3& right, Vector3& up, Vector3& forward) const;
	//	void					GetDirection(Vector3& forward) const;

		float					Dot(const Quaternion& quat) const;

		float&					operator[](int index);
		float					operator[](int index) const;
		Quaternion&				operator =(const Quaternion& src);
		Quaternion&				operator+=(const Quaternion& quat);
		Quaternion&				operator-=(const Quaternion& quat);
		Quaternion&				operator*=(const Quaternion& quat);					// Hamilton product
		Quaternion&				operator*=(float scale);
	//	Quaternion&				operator/=(const Quaternion& quat);
		Quaternion&				operator/=(float factor);

		bool					operator==(const Quaternion& quat) const;
		bool					operator!=(const Quaternion& quat) const;
		Quaternion				operator +(const Quaternion& quat) const;
		Quaternion				operator -(const Quaternion& quat) const;
		Quaternion				operator -(void) const;
		Quaternion				operator *(const Quaternion& quat) const;			// Hamilton product
		Vector3					operator *(const Vector3 &vec) const;				// Vector rotation
		Quaternion				operator *(float scale) const;
	//	Quaternion				operator /(const Quaternion& quat) const;
		Quaternion				operator /(float factor) const;
		friend Quaternion		operator *(float scale, const Quaternion& quat);

		static Quaternion		Lerp(const Quaternion& orig, const Quaternion& dest, float time);
		static Quaternion		Slerp(const Quaternion& orig, const Quaternion& dest, float time);

		static const Quaternion	IDENTITY;

	private:
		union
		{
			float	m_data[4];
		//	struct	{ Vector3 imaginary, float real };
		//	struct	{ float x, y, z, w; };
		//	__m128	m_mm_data;
		};

	};

	//	----------------------------------------------------------
	inline Quaternion::Quaternion(const Quaternion& src)
	{
		m_data[X] = src[X];
		m_data[Y] = src[Y];
		m_data[Z] = src[Z];
		m_data[W] = src[W];
	}

	inline Quaternion::Quaternion(float x, float y, float z, float w)
	{
		m_data[X] = x;
		m_data[Y] = y;
		m_data[Z] = z;
		m_data[W] = w;
	}

	inline void Quaternion::Set(const Vector3& axis, float radians)
	{
		const float half_angle = radians * 0.5f;
		const float sine = Math::Sin(half_angle);
		m_data[X] = axis[X] * sine;
		m_data[Y] = axis[Y] * sine;
		m_data[Z] = axis[Z] * sine;
		m_data[W] = Math::Cos(half_angle);
	}

	inline void Quaternion::Set(const Vector3& orig, const Vector3& dest)
	{
		//http://physicsforgames.blogspot.com/2010/03/quaternion-tricks.html
		Vector3 half(orig);
		half += dest;
		half.Normalize(); //since we're gonna normalize the vector anyway, don't bother to divide it by 2.

		//Eric Brown's sample might be wrong... orig and dest seem to be swapped.
		//m_data[X] = orig[Y] * half[Z] - orig[Z] * half[Y];
		//m_data[Y] = orig[Z] * half[X] - orig[X] * half[Z];
		//m_data[Z] = orig[X] * half[Y] - orig[Y] * half[X];
		//m_data[W] = orig.Dot(half);
		m_data[X] = dest[Y] * half[Z] - dest[Z] * half[Y];
		m_data[Y] = dest[Z] * half[X] - dest[X] * half[Z];
		m_data[Z] = dest[X] * half[Y] - dest[Y] * half[X];
		m_data[W] = dest.Dot(half);
	}

	inline void Quaternion::Set(const Vector3& right, const Vector3& up, const Vector3& forward)
	{
		//http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
		//http://www.gamasutra.com/view/feature/3278/rotating_objects_using_quaternions.php
		float trace;

		trace = right[X] + up[Y] + forward[Z];
		if (trace > 0.0f)
		{
			float s = 0.5f / Math::SqRt(1.0f + trace);

			m_data[X] = (forward[Y] -      up[Z]) * s;
			m_data[Y] = (  right[Z] - forward[X]) * s;
			m_data[Z] = (     up[X] -   right[Y]) * s;
			m_data[W] = 0.25f / s;
		}
		else
		{
			if (right[X] > up[Y] && right[X] > forward[Z])
			{
				float s = 0.5f / Math::SqRt(1.0f + right[X] - up[Y] - forward[Z]);

				m_data[Y] = (right[Y] +      up[X]) * s;
				m_data[Z] = (right[Z] + forward[X]) * s;
				m_data[W] = (   up[Z] - forward[Y]) * s;
				m_data[X] = 0.25f  / s;
			}
			else if (up[Y] > forward[Z])
			{
				float s = 0.5f / Math::SqRt(1.0f + up[Y] - right[X] - forward[Z]);

				m_data[Z] = (   up[Z] + forward[Y]) * s;
				m_data[W] = (right[Z] - forward[X]) * s;
				m_data[X] = (right[Y] +      up[X]) * s;
				m_data[Y] = 0.25f / s;
			}
			else
			{
				float s = 0.5f / Math::SqRt(1.0f + forward[Z] - right[X] - up[Y]);

				m_data[W] = (right[Y] -      up[X]) * s;
				m_data[X] = (right[Z] + forward[X]) * s;
				m_data[Y] = (   up[Z] + forward[Y]) * s;
				m_data[Z] = 0.25f / s;
			}
		}
	}

	inline void Quaternion::Normalize(void)
	{
		float norm = GetNorm();
		if (norm > 0.0f && norm != 1.0f)
		{
			*this /= Math::SqRt(norm);
		}
	}

	inline void Quaternion::Conjugate(void)
	{
		m_data[X] = -m_data[X];
		m_data[Y] = -m_data[Y];
		m_data[Z] = -m_data[Z];
	}

	inline void Quaternion::Invert(void)
	{
		Conjugate();
		*this /= GetNorm();
	}

	inline float Quaternion::GetNorm(void) const
	{
		return SQUARE(m_data[X]) + SQUARE(m_data[Y]) + SQUARE(m_data[Z]) + SQUARE(m_data[W]);
	}

	inline Quaternion Quaternion::GetConjugate(void) const
	{
		return Quaternion(-m_data[X], -m_data[Y], -m_data[Z], m_data[W]);
	}

	inline Quaternion Quaternion::GetInverse(void) const
	{
		return GetConjugate() / GetNorm();
	}

	inline void Quaternion::GetAxes(Vector3& right, Vector3& up, Vector3& forward) const
	{
		//http://www.martinb.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
		//http://www.gamasutra.com/view/feature/3278/rotating_objects_using_quaternions.php?page=2

		// calculate coefficients
		float x2 = m_data[X] + m_data[X];
		float y2 = m_data[Y] + m_data[Y];
		float z2 = m_data[Z] + m_data[Z];
		float xx = m_data[X] * x2;
		float xy = m_data[X] * y2;
		float xz = m_data[X] * z2;
		float yy = m_data[Y] * y2;
		float yz = m_data[Y] * z2;
		float zz = m_data[Z] * z2;
		float wx = m_data[W] * x2;
		float wy = m_data[W] * y2;
		float wz = m_data[W] * z2;

		right[X] = 1.0f - (yy + zz);
		right[Y] = xy - wz;
		right[Z] = xz + wy;

		up[X] = xy + wz;
		up[Y] = 1.0f - (xx + zz);
		up[Z] = yz - wx;

		forward[X] = xz - wy;
		forward[Y] = yz + wx;
		forward[Z] = 1.0f - (xx + yy);
	}

	/*
	inline void Quaternion::GetDirection(Vector3& forward) const
	{
		//http://www.martinb.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
		//http://www.gamasutra.com/view/feature/3278/rotating_objects_using_quaternions.php?page=2

		// calculate coefficients
		float x2 = m_data[X] + m_data[X];
		float y2 = m_data[Y] + m_data[Y];
		float z2 = m_data[Z] + m_data[Z];
		float xx = m_data[X] * x2;
		//float xy = m_data[X] * y2;
		float xz = m_data[X] * z2;
		float yy = m_data[Y] * y2;
		float yz = m_data[Y] * z2;
		//float zz = m_data[Z] * z2;
		float wx = m_data[W] * x2;
		float wy = m_data[W] * y2;
		//float wz = m_data[W] * z2;

		forward[X] = xz - wy;
		forward[Y] = yz + wx;
		forward[Z] = 1.0f - (xx + yy);
	}
	*/

	inline float Quaternion::Dot(const Quaternion& quat) const
	{
		return (m_data[X] * quat[X]) + (m_data[Y] * quat[Y]) + (m_data[Z] * quat[Z]) + (m_data[W] * quat[W]);
	}

	inline float& Quaternion::operator[](int index)
	{
		return m_data[index];
	}

	inline float Quaternion::operator[](int index) const
	{
		return m_data[index];
	}

	inline Quaternion& Quaternion::operator=(const Quaternion& src)
	{
		if (this != &src) // optimization: ignore self-assignement
		{
			m_data[X] = src[X];
			m_data[Y] = src[Y];
			m_data[Z] = src[Z];
			m_data[W] = src[W];
		}
		return *this;
	}

	inline Quaternion& Quaternion::operator+=(const Quaternion& quat)
	{
		m_data[X] += quat[X];
		m_data[Y] += quat[Y];
		m_data[Z] += quat[Z];
		m_data[W] += quat[W];

		return *this;
	}

	inline Quaternion& Quaternion::operator-=(const Quaternion& quat)
	{
		m_data[X] -= quat[X];
		m_data[Y] -= quat[Y];
		m_data[Z] -= quat[Z];
		m_data[W] -= quat[W];

		return *this;
	}

	inline Quaternion& Quaternion::operator*=(const Quaternion& quat)
	{
		//store compoments of each operand in local variables so they can be optimized by compiler
		float q1w = m_data[W], q1x = m_data[X], q1y = m_data[Y], q1z = m_data[Z];
		float q2w =   quat[W], q2x =   quat[X], q2y =   quat[Y], q2z =   quat[Z];

		//http://en.wikipedia.org/wiki/Quaternion
		m_data[X] = (q1w * q2x) + (q1x * q2w) + (q1y * q2z) - (q1z * q2y);
		m_data[Y] = (q1w * q2y) - (q1x * q2z) + (q1y * q2w) + (q1z * q2x);
		m_data[Z] = (q1w * q2z) + (q1x * q2y) - (q1y * q2x) + (q1z * q2w);
		m_data[W] = (q1w * q2w) - (q1x * q2x) - (q1y * q2y) - (q1z * q2z);

		return *this;
	}

	inline Quaternion& Quaternion::operator*=(float scale)
	{
		m_data[X] *= scale;
		m_data[Y] *= scale;
		m_data[Z] *= scale;
		m_data[W] *= scale;

		return *this;
	}

	//inline Quaternion& Quaternion::operator/=(const Quaternion& quat)
	//{
	//	//STUB
	//	return *this;
	//}

	inline Quaternion& Quaternion::operator/=(float factor)
	{
		float scale = 1.0f / factor;
		m_data[X] *= scale;
		m_data[Y] *= scale;
		m_data[Z] *= scale;
		m_data[W] *= scale;

		return *this;
	}

	inline bool Quaternion::operator==(const Quaternion& quat) const
	{
		return ((m_data[X] == quat[X]) && (m_data[Y] == quat[Y]) && (m_data[Z] == quat[Z]) && (m_data[W] == quat[W]));
	}

	inline bool Quaternion::operator!=(const Quaternion& quat) const
	{
		return ((m_data[X] != quat[X]) || (m_data[Y] != quat[Y]) || (m_data[Z] != quat[Z]) || (m_data[W] != quat[W]));
	}

	inline Quaternion Quaternion::operator+(const Quaternion& quat) const
	{
		return Quaternion(m_data[X] + quat[X], m_data[Y] + quat[Y], m_data[Z] + quat[Z], m_data[W] + quat[W]);
	}

	inline Quaternion Quaternion::operator-(const Quaternion& quat) const
	{
		return Quaternion(m_data[X] - quat[X], m_data[Y] - quat[Y], m_data[Z] - quat[Z], m_data[W] - quat[W]);
	}

	inline Quaternion Quaternion::operator-(void) const
	{
		return Quaternion(-m_data[X], -m_data[Y], -m_data[Z], -m_data[W]);
	}

	inline Quaternion Quaternion::operator*(const Quaternion& quat) const
	{
		//store compoments of each operand in local variables so they can be optimized by compiler
		float q1w = m_data[W], q1x = m_data[X], q1y = m_data[Y], q1z = m_data[Z];
		float q2w =   quat[W], q2x =   quat[X], q2y =   quat[Y], q2z =   quat[Z];

		//http://en.wikipedia.org/wiki/Quaternion
		return Quaternion(
			(q1w * q2x) + (q1x * q2w) + (q1y * q2z) - (q1z * q2y),
			(q1w * q2y) - (q1x * q2z) + (q1y * q2w) + (q1z * q2x),
			(q1w * q2z) + (q1x * q2y) - (q1y * q2x) + (q1z * q2w),
			(q1w * q2w) - (q1x * q2x) - (q1y * q2y) - (q1z * q2z)
		);
	}

	inline Vector3 Quaternion::operator*(const Vector3 &vec) const
	{
#ifdef _DEBUG
		//store compoments of each operand in local variables so they can be optimized by compiler
		const float vx =    vec[X], vy =    vec[Y], vz =    vec[Z];
		const float qx = m_data[X], qy = m_data[Y], qz = m_data[Z], qw = m_data[W];

		//http://physicsforgames.blogspot.com/2010/03/quaternion-tricks.html
		float x1 = qy * vz - qz * vy;
		float y1 = qz * vx - qx * vz;
		float z1 = qx * vy - qy * vx;

		float x2 = qw * x1 + qy * z1 - qz * y1;
		float y2 = qw * y1 + qz * x1 - qx * z1;
		float z2 = qw * z1 + qx * y1 - qy * x1;
#else
		//http://physicsforgames.blogspot.com/2010/03/m_dataernion-tricks.html
		float x1 = m_data[Y] * vec[Z] - m_data[Z] * vec[Y];
		float y1 = m_data[Z] * vec[X] - m_data[X] * vec[Z];
		float z1 = m_data[X] * vec[Y] - m_data[Y] * vec[X];

		float x2 = m_data[W] * x1 + m_data[Y] * z1 - m_data[Z] * y1;
		float y2 = m_data[W] * y1 + m_data[Z] * x1 - m_data[X] * z1;
		float z2 = m_data[W] * z1 + m_data[X] * y1 - m_data[Y] * x1;
#endif
		return Vector3(
			vec[X] + (x2 + x2),
			vec[Y] + (y2 + y2),
			vec[Z] + (z2 + z2)
		);
	}

	inline Quaternion Quaternion::operator*(float scaler) const
	{
		return Quaternion(m_data[X] * scaler, m_data[Y] * scaler, m_data[Z] * scaler, m_data[W] * scaler);
	}

	inline Quaternion operator*(float scale, const Quaternion& quat)
	{
		return quat * scale;
	}

	//inline Quaternion Quaternion::operator/(const Quaternion& quat) const
	//{
	//	//STUB
	//	factor = 1.0f / factor;
	//	return Quaternion(m_data[X] * factor, m_data[Y] * factor, m_data[Z] * factor, m_data[W] * factor);
	//}

	inline Quaternion Quaternion::operator/(float factor) const
	{
		factor = 1.0f / factor;
		return Quaternion(m_data[X] * factor, m_data[Y] * factor, m_data[Z] * factor, m_data[W] * factor);
	}

	inline Quaternion Quaternion::Lerp(const Quaternion& orig, const Quaternion& dest, float t)
	{
		Quaternion delta = dest - orig;
		delta *= t;
		delta += orig;
		return delta;
	}

	inline Quaternion Quaternion::Slerp(const Quaternion& orig, const Quaternion& dest, float t)
	{
		Quaternion target;
		float dot = orig.Dot(dest);

		// orig and dest are in opposite directions - let's reduce the unnecessary spinning
		if (dot < 0.0f)
		{
			dot = -dot;
			target = -dest;
		}
		else
		{
			target = dest;
		}

		const float dot_threshold = 0.01f;

		if (dot >= 1.0f - dot_threshold)
		{
			// the angle is pretty small, just use lerp instead
			return Lerp(orig, target, t);
		}
		else
		{
			const float angle = Math::ACos(dot);
			return (orig * Math::Sin(angle * (1.0f - t)) + target * Math::Sin(angle * t)) / Math::Sin(angle);
		}
	}

}	// namespace Math

#endif // __QUATERNION_H__
