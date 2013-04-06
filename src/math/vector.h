/*************************************************************
 *	Vector
 *	by Rick Juang, 2011/07/20
 *	Vector math library
 *************************************************************/
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math/math.h>
//#include <utility/debug.h>	// for Dbg_Assert

namespace Math
{
	class Quaternion;

	class Vector3
	{
	public:
		Vector3(void) {}														// Do nothing for speedy creation
		Vector3(const Vector3& src);
		Vector3(float x, float y, float z);

		float					GetLength(void) const;
		float					GetLengthSq(void) const;

		void					Zero(void);

		void					Normalize(void);
		void					Constrain(float min_length, float max_length);
		void					Resize(float length);
		void					Scale(const Vector3& scale);
		void					Project(const Vector3& normal);
		void					Reflect(const Vector3& normal);
		void					Rotate(const Vector3& unit_axis, float radians);
		void					Rotate(const Quaternion& quat);
	//	void					Rotate(const Matrix& mat);

		Vector3					GetNormalized(void) const;
		Vector3					GetConstrained(float min_length, float max_length) const;
		Vector3					GetResized(float length) const;
		Vector3					GetScaled(const Vector3& scale) const;
		Vector3					GetProjected(const Vector3& normal) const;
		Vector3					GetReflected(const Vector3& normal) const;
		Vector3					GetRotated(const Vector3& unit_axis, float radians) const;
		Vector3					GetRotated(const Quaternion& quat) const;
	//	Vector3					GetRotated(const Matrix& mat) const;

		float					Dot(const Vector3& vec) const;
		Vector3					Cross(const Vector3& vec) const;

		float&					operator[](int index);
		float					operator[](int index) const;
		Vector3&				operator =(const Vector3& src);
		Vector3&				operator+=(const Vector3& vec);
		Vector3&				operator-=(const Vector3& vec);
	//	Vector3&				operator*=(const Vector3& scale);
		Vector3&				operator*=(float scale);
		Vector3&				operator/=(float factor);

		bool					operator==(const Vector3& vec) const;
		bool					operator!=(const Vector3& vec) const;
		Vector3					operator +(const Vector3& vec) const;
		Vector3					operator -(const Vector3& vec) const;
		Vector3					operator -(void) const;
	//	float					operator *(const Vector3& vec) const;						// dot product
	//	float					operator *(const Vector3& scale) const;						// vector scale
		Vector3					operator *(float scale) const;
		Vector3					operator /(float factor) const;
	//	friend Vector3			operator *(float scale, const Vector3& vec);
		friend Vector3			operator /(float dividend, const Vector3& factor);

	//	void					Lerp(const Vector3& orig, const Vector3& dest, float time);	// lerp and store result in this vector
		static Vector3			Lerp(const Vector3& orig, const Vector3& dest, float time);
		static float			Theta(const Vector3& orig, const Vector3& dest);			// angle between 2 vectors

		static const Vector3	ZERO;
		static const Vector3	ONE;
		static const Vector3	UNIT_X;
		static const Vector3	UNIT_Y;
		static const Vector3	UNIT_Z;

	private:
		union
		{
			float	m_data[Z + 1];
		//	struct	{ float x, y, z; };
		//	__m128	m_mm_data;
		};
	};

	//	----------------------------------------------------------
	inline Vector3::Vector3(const Vector3& src)
	{
		m_data[X] = src[X];
		m_data[Y] = src[Y];
		m_data[Z] = src[Z];
	}

	inline Vector3::Vector3(float x, float y, float z)
	{
		m_data[X] = x;
		m_data[Y] = y;
		m_data[Z] = z;
	}

	inline float Vector3::GetLength(void) const
	{
		return Math::SqRt(GetLengthSq());
	}

	inline float Vector3::GetLengthSq(void) const
	{
		return (m_data[X] * m_data[X]) + (m_data[Y] * m_data[Y]) + (m_data[Z] * m_data[Z]);
	}

	inline void Vector3::Zero(void)
	{
		m_data[X] = m_data[Y] = m_data[Z] = 0.0f;
	}

	inline void Vector3::Normalize(void)
	{
		const float len_sq = GetLengthSq();
		if (len_sq > 0.0f && len_sq != 1.0f)
		{
			*this /= Math::SqRt(len_sq);
		}
	}

	inline void Vector3::Constrain(float min_length, float max_length)
	{
		const float len_sq = GetLengthSq();
		const float min_len_sq = SQUARE(min_length);
		const float max_len_sq = SQUARE (max_length);

		if (len_sq < min_len_sq)
		{
			*this *= (min_length / Math::SqRt(len_sq));
		}
		else if (len_sq > max_len_sq)
		{
			*this *= (max_length / Math::SqRt(len_sq));
		}
	}

	inline void Vector3::Resize(float length)
	{
		Normalize();
		*this *= length;
	}

	inline void Vector3::Scale(const Vector3& scale)
	{
		m_data[X] *= scale[X];
		m_data[Y] *= scale[Y];
		m_data[Z] *= scale[Z];
	}

	inline void Vector3::Project(const Vector3& normal)
	{
		*this = GetProjected(normal);
	}

	inline void Vector3::Reflect(const Vector3& normal)
	{
		*this = GetReflected(normal);
	}

	inline Vector3 Vector3::GetNormalized(void) const
	{
		Vector3 normalized(*this);
		normalized.Normalize();
		return normalized;
	}

	inline Vector3 Vector3::GetConstrained(float min_length, float max_length) const
	{
		Vector3 constrained(*this);
		constrained.Constrain(min_length, max_length);
		return constrained;
	}

	inline Vector3 Vector3::GetResized(float length) const
	{
		Vector3 resized(*this);
		resized.Resize(length);
		return resized;
	}

	inline Vector3 Vector3::GetScaled(const Vector3& scale) const
	{
		Vector3 scaled(*this);
		scaled.Scale(scale);
		return scaled;
	}

	inline Vector3 Vector3::GetProjected(const Vector3& normal) const
	{
		Vector3 projected(normal);
		projected.Normalize();
		projected *= Dot(normal);
		return projected;
	}

	inline Vector3 Vector3::GetReflected(const Vector3& normal) const
	{
		return Vector3(GetProjected(normal) * 2 - *this);
	}

	inline Vector3 Vector3::GetRotated(const Vector3& unit_axis, float radians) const
	{
		Vector3 rotated(*this);
		rotated.Rotate(unit_axis, radians);
		return rotated;
	}

	inline Vector3 Vector3::GetRotated(const Quaternion& quat) const
	{
		Vector3 rotated(*this);
		rotated.Rotate(quat);
		return rotated;
	}

	inline float Vector3::Dot(const Vector3& vec) const
	{
		return (m_data[X] * vec[X]) + (m_data[Y] * vec[Y]) + (m_data[Z] * vec[Z]);
	}

	inline Vector3 Vector3::Cross(const Vector3& vec) const
	{
		return Vector3(
			(m_data[Y] * vec[Z]) - (m_data[Z] * vec[Y]),
			(m_data[Z] * vec[X]) - (m_data[X] * vec[Z]),
			(m_data[X] * vec[Y]) - (m_data[Y] * vec[X])
		);
	}

	inline float& Vector3::operator[](int index)
	{
		return m_data[index];
	}

	inline float Vector3::operator[](int index) const
	{
		return m_data[index];
	}

	inline Vector3& Vector3::operator=(const Vector3& src)
	{
		if (this != &src) // optimization: ignore self-assignement
		{
			m_data[X] = src[X];
			m_data[Y] = src[Y];
			m_data[Z] = src[Z];
		}
		return *this;
	}

	inline Vector3& Vector3::operator+=(const Vector3& vec)
	{
		m_data[X] += vec[X];
		m_data[Y] += vec[Y];
		m_data[Z] += vec[Z];

		return *this;
	}

	inline Vector3& Vector3::operator-=(const Vector3& vec)
	{
		m_data[X] -= vec[X];
		m_data[Y] -= vec[Y];
		m_data[Z] -= vec[Z];

		return *this;
	}

	inline Vector3& Vector3::operator*=(float scale)
	{
		m_data[X] *= scale;
		m_data[Y] *= scale;
		m_data[Z] *= scale;

		return *this;
	}

	/*
	inline Vector3& Vector3::operator*=(const Vector3& scale)
	{
		m_data[X] *= scale[X];
		m_data[Y] *= scale[Y];
		m_data[Z] *= scale[Z];

		return *this;
	}
	*/

	inline Vector3& Vector3::operator/=(float factor)
	{
		float scale = 1.0f / factor;
		m_data[X] *= scale;
		m_data[Y] *= scale;
		m_data[Z] *= scale;

		return *this;
	}

	inline bool Vector3::operator==(const Vector3& vec) const
	{
		return ((m_data[X] == vec[X]) && (m_data[Y] == vec[Y]) && (m_data[Z] == vec[Z]));
	}

	inline bool Vector3::operator!=(const Vector3& vec) const
	{
		return ((m_data[X] != vec[X]) || (m_data[Y] != vec[Y]) || (m_data[Z] != vec[Z]));
	}

	inline Vector3 Vector3::operator+(const Vector3& vec) const
	{
		return Vector3(m_data[X] + vec[X], m_data[Y] + vec[Y], m_data[Z] + vec[Z]);
	}

	inline Vector3 Vector3::operator-(const Vector3& vec) const
	{
		return Vector3(m_data[X] - vec[X], m_data[Y] - vec[Y], m_data[Z] - vec[Z]);
	}

	inline Vector3 Vector3::operator-(void) const
	{
		return Vector3(-m_data[X], -m_data[Y], -m_data[Z]);
	}

	/*
	inline float Vector3::operator*(const Vector3& vec) const
	{
		return this->Dot(vec);
	}
	*/

	/*
	inline float Vector3::operator*(const Vector3& scale) const
	{
		return Vector3(m_data[X] * scale[X], m_data[Y] * scale[Y], m_data[Z] * scale[Z]);
	}
	*/

	inline Vector3 Vector3::operator*(float scale) const
	{
		return Vector3(m_data[X] * scale, m_data[Y] * scale, m_data[Z] * scale);
	}

	/*
	inline Vector3 operator*(float scale, const Vector3& vec)
	{
		return vec * scale;
	}
	*/

	inline Vector3 operator/(float dividend, const Vector3& factor)
	{
		return Vector3(dividend / factor[X], dividend / factor[Y], dividend / factor[Z]);
	}

	inline Vector3 Vector3::operator/(float factor) const
	{
		factor = 1.0f / factor;
		return Vector3(m_data[X] * factor, m_data[Y] * factor, m_data[Z] * factor);
	}

	inline Vector3 Vector3::Lerp(const Vector3& orig, const Vector3& dest, float t)
	{
		Vector3 delta = dest - orig;
		delta *= t;
		delta += orig;
		return delta;
	}

	inline float Vector3::Theta(const Vector3& orig, const Vector3& dest)
	{
		return Math::ACos(dest.GetNormalized().Dot(orig.GetNormalized()));
	}

}	// namespace Math

#endif // __VECTOR_H__
