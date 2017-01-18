
#include "vector2.h"

Vector2::Vector2(float x, float y)
{
	Vector2::x = x;
	Vector2::y = y;
}

Vector2::Vector2(void)
{
	Vector2::x = 0;
	Vector2::y = 0;
}

void Vector2::operator +=(Vector2 InVector)
{
	x += InVector.x;
	y += InVector.y;
}

void Vector2::operator -=(Vector2 InVector)
{
	x -= InVector.x;
	y -= InVector.y;
}

void Vector2::operator *=(Vector2 InVector)
{
	x *= InVector.x;
	y *= InVector.y;
}

void Vector2::operator /=(Vector2 InVector)
{
	x /= InVector.x;
	y /= InVector.y;
}

void Vector2::operator +=(float value)
{
	x += value;
	y += value;
}

void Vector2::operator -=(float value)
{
	x -= value;
	y -= value;
}

void Vector2::operator *=(float value)
{
	x *= value;
	y *= value;
}

void Vector2::operator /=(float value)
{
	x /= value;
	y /= value;
}

Vector2 Vector2::operator +(Vector2 InVector)
{
	Vector2 OutVector;

	OutVector.x = x + InVector.x;
	OutVector.y = y + InVector.y;

	return OutVector;
}

Vector2 Vector2::operator -(Vector2 InVector)
{
	Vector2 OutVector;

	OutVector.x = x - InVector.x;
	OutVector.y = y - InVector.y;

	return OutVector;
}

Vector2 Vector2::operator *(Vector2 InVector)
{
	Vector2 OutVector;

	OutVector.x = x * InVector.x;
	OutVector.y = y * InVector.y;

	return OutVector;
}

Vector2 Vector2::operator /(Vector2 InVector)
{
	Vector2 OutVector;

	OutVector.x = x / InVector.x;
	OutVector.y = y / InVector.y;

	return OutVector;
}

Vector2 Vector2::operator +(float value)
{
	Vector2 OutVector;

	OutVector.x = x + value;
	OutVector.y = y + value;

	return OutVector;
}

Vector2 Vector2::operator -(float value)
{
	Vector2 OutVector;

	OutVector.x = x - value;
	OutVector.y = y - value;

	return OutVector;
}

Vector2 Vector2::operator *(float value)
{
	Vector2 OutVector;

	OutVector.x = x * value;
	OutVector.y = y * value;

	return OutVector;
}

Vector2 Vector2::operator /(float value)
{
	Vector2 OutVector;

	OutVector.x = x / value;
	OutVector.y = y / value;

	return OutVector;
}

void Vector2::operator =(Vector2 InVector)
{
	x = InVector.x;
	y = InVector.y;
}

void Vector2::operator =(float value)
{
	x = value;
	y = value;
}

int Vector2::operator ==(Vector2 InVector)
{
	if(InVector.x = x)
		if(InVector.y = y)
			return (1);
	return (0);
}
int Vector2::operator !=(Vector2 InVector)
{
	if(InVector.x = x)
		if(InVector.y = y)
			return (0);
	return (1);
}

int Vector2::operator ==(float value)
{
	if(value = x)
		if(value = y)
			return (1);
	return (0);
}
int Vector2::operator !=(float value)
{
	if(value = x)
		if(value = y)
			return (0);
	return (1);
}

void Vector2::Normalize()
{
	float length = sqrtf(x * x + y * y);
	x /= length;
	y /= length;
}