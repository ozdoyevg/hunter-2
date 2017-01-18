
#include "vector3.h"

Vector3::Vector3(float x, float y, float z)
{
	Vector3::x = x;
	Vector3::y = y;
	Vector3::z = z;
}

Vector3::Vector3(void)
{
	Vector3::x = 0;
	Vector3::y = 0;
	Vector3::z = 0;
}

void Vector3::operator +=(Vector3 InVector)
{
	x += InVector.x;
	y += InVector.y;
	z += InVector.z;
}

void Vector3::operator -=(Vector3 InVector)
{
	x -= InVector.x;
	y -= InVector.y;
	z -= InVector.z;
}

void Vector3::operator *=(Vector3 InVector)
{
	x *= InVector.x;
	y *= InVector.y;
	z *= InVector.z;
}

void Vector3::operator /=(Vector3 InVector)
{
	x /= InVector.x;
	y /= InVector.y;
	z /= InVector.z;
}

void Vector3::operator +=(float value)
{
	x += value;
	y += value;
	z += value;
}

void Vector3::operator -=(float value)
{
	x -= value;
	y -= value;
	z -= value;
}

void Vector3::operator *=(float value)
{
	x *= value;
	y *= value;
	z *= value;
}

void Vector3::operator /=(float value)
{
	x /= value;
	y /= value;
	z /= value;
}

Vector3 Vector3::operator +(Vector3 InVector)
{
	Vector3 OutVector;

	OutVector.x = x + InVector.x;
	OutVector.y = y + InVector.y;
	OutVector.z = z + InVector.z;

	return OutVector;
}

Vector3 Vector3::operator -(Vector3 InVector)
{
	Vector3 OutVector;

	OutVector.x = x - InVector.x;
	OutVector.y = y - InVector.y;
	OutVector.z = z - InVector.z;

	return OutVector;
}

Vector3 Vector3::operator-() {
	return Vector3(-x, -y, -z);
}

float Vector3::operator *(Vector3 InVector)
{
	return (x * InVector.x) + (y * InVector.y) + (z * InVector.z);
}

Vector3 Vector3::operator ^(Vector3 InVector)
{
	Vector3 OutVector;

	OutVector.x = y * InVector.z - z * InVector.y;
	OutVector.y = z * InVector.x - x * InVector.z;
	OutVector.z = x * InVector.y - y * InVector.x;

	return OutVector;
}

Vector3 Vector3::operator +(float value)
{
	Vector3 OutVector;

	OutVector.x = x + value;
	OutVector.y = y + value;
	OutVector.z = z + value;

	return OutVector;
}

Vector3 Vector3::operator -(float value)
{
	Vector3 OutVector;

	OutVector.x = x - value;
	OutVector.y = y - value;
	OutVector.z = z - value;

	return OutVector;
}

Vector3 Vector3::operator *(float value)
{
	Vector3 OutVector;

	OutVector.x = x * value;
	OutVector.y = y * value;
	OutVector.z = z * value;

	return OutVector;
}

Vector3 Vector3::operator /(float value)
{
	Vector3 OutVector;

	OutVector.x = x / value;
	OutVector.y = y / value;
	OutVector.z = z / value;

	return OutVector;
}

void Vector3::operator =(Vector3 InVector)
{
	x = InVector.x;
	y = InVector.y;
	z = InVector.z;
}

void Vector3::operator =(float value)
{
	x = value;
	y = value;
	z = value;
}

int Vector3::operator ==(Vector3 InVector)
{
	if(InVector.x == x && InVector.y == y && InVector.z == z)
		return (1);
	return (0);
}
int Vector3::operator !=(Vector3 InVector)
{
	if(InVector.x == x)
		if(InVector.y == y)
			if(InVector.z == z)
				return (0);
	return (1);
}

int Vector3::operator ==(float value)
{
	if(value = x)
		if(value = y)
			if(value = z)
				return (1);
	return (0);
}
int Vector3::operator !=(float value)
{
	if(value = x)
		if(value = y)
			if(value = z)
				return (0);
	return (1);
}

Vector3 Vector3::Normal() const
{
	float length = sqrtf(x * x + y * y + z * z);
	if (length == 0)
		return Vector3(0.0f, 0.0f, 0.0f);
	return Vector3(x / length, y / length, z / length);
}

void Vector3::Normalize()
{
	float length = sqrtf(x * x + y * y + z * z);
	if (length == 0) {
		x = 0;
		y = 0;
		z = 0;
	}
	x /= length;
	y /= length;
	z /= length;
}

float Vector3::Length()
{
	return sqrtf(x * x + y * y + z * z);
}

float Vector3::PlaneDistance(Vector3 vPlane, Vector3 vNormal) {
	return Vector3(x - vPlane.x, y - vPlane.y, z - vPlane.z) * vNormal;
}