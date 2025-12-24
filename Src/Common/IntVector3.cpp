#include "IntVector3.h"

IntVector3::IntVector3(void)
{
	x = 0;
	y = 0;
	z = 0;
}

IntVector3::IntVector3(int vX, int vY, int vZ)
{
	x = vX;
	y = vY;
	z = vZ;
}

IntVector3::~IntVector3(void)
{
}

const IntVector3 IntVector3::operator+(const IntVector3 value)const
{
	return { x + value.x , y + value.y, z + value.z};
}

const IntVector3 IntVector3::operator+(const int value) const
{
	return{ x + value,y + value ,z + value };
}

void IntVector3::operator+=(const IntVector3 value)
{
	x += value.x;
	y += value.y;
	z += value.z;
}

const IntVector3 IntVector3::operator-(const IntVector3 value)const
{
	return { x - value.x , y - value.y, z - value.z };
}

void IntVector3::operator-=(const IntVector3 value)
{
	x -= value.x;
	y -= value.y;
	z -= value.z;
}

const IntVector3 IntVector3::operator*(const int value)const
{
	return { x * value , y * value, z * value };
}

void IntVector3::operator*=(const int value)
{
	x *= value;
	y *= value;
	z *= value;
}

const IntVector3 IntVector3::operator/(const int value)const
{
	return { x / value , y / value, z / value };
}

void IntVector3::operator/=(const int value)
{
	x /= value;
	y /= value;
	z /= value;
}
