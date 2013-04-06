/*************************************************************
 *	Vector
 *	by Rick Juang, 2011/07/20
 *	Vector math library
 *************************************************************/

#include "vector.h"
#include "quaternion.h"

namespace Math
{

const Vector3	Vector3::ZERO	(0.0f, 0.0f, 0.0f);
const Vector3	Vector3::ONE	(1.0f, 1.0f, 1.0f);
const Vector3	Vector3::UNIT_X	(1.0f, 0.0f, 0.0f);
const Vector3	Vector3::UNIT_Y	(0.0f, 1.0f, 0.0f);
const Vector3	Vector3::UNIT_Z	(0.0f, 0.0f, 1.0f);

void Vector3::Rotate(const Vector3& unit_axis, float radians)
{
	Rotate(Quaternion(unit_axis, radians));
}


void Vector3::Rotate(const Quaternion& quat)
{
#if 1
	//store compoments of each operand in local variables so they can be optimized by compiler
	const float vx = m_data[X], vy = m_data[Y], vz = m_data[Z];
	const float qx =   quat[X], qy =   quat[Y], qz =   quat[Z],   qw = quat[W];

	//http://physicsforgames.blogspot.com/2010/03/quaternion-tricks.html
	float x1 = qy * vz - qz * vy;
	float y1 = qz * vx - qx * vz;
	float z1 = qx * vy - qy * vx;

	float x2 = qw * x1 + qy * z1 - qz * y1;
	float y2 = qw * y1 + qz * x1 - qx * z1;
	float z2 = qw * z1 + qx * y1 - qy * x1;
#else
	//http://physicsforgames.blogspot.com/2010/03/quaternion-tricks.html
	float x1 = quat[Y] * m_data[Z] - quat[Z] * m_data[Y];
	float y1 = quat[Z] * m_data[X] - quat[X] * m_data[Z];
	float z1 = quat[X] * m_data[Y] - quat[Y] * m_data[X];

	float x2 = quat[W] * x1 + quat[Y] * z1 - quat[Z] * y1;
	float y2 = quat[W] * y1 + quat[Z] * x1 - quat[X] * z1;
	float z2 = quat[W] * z1 + quat[X] * y1 - quat[Y] * x1;
#endif
	m_data[X] += (x2 + x2);
	m_data[Y] += (y2 + y2);
	m_data[Z] += (z2 + z2);
}

}	// Namespace Math
