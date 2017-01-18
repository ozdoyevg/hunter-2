
#pragma once

#include <math.h>

class Vector2
{ 
public:

	Vector2(float x, float y);
	Vector2();

	void operator +=(Vector2);
	void operator -=(Vector2);
	void operator *=(Vector2);
	void operator /=(Vector2);

	void operator +=(float);
	void operator -=(float);
	void operator *=(float);
	void operator /=(float);

	Vector2 operator +(Vector2);
	Vector2 operator -(Vector2);
	Vector2 operator *(Vector2);
	Vector2 operator /(Vector2);

	Vector2 operator +(float);
	Vector2 operator -(float);
	Vector2 operator *(float);
	Vector2 operator /(float);

	void operator =(Vector2);

	void operator =(float);

	int operator ==(Vector2);
	int operator !=(Vector2);

	int operator ==(float);
	int operator !=(float);

	void Normalize();
public:

	float x;
	float y;
};