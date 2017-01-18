
#pragma once

#include <math.h>

class Vector3
{ 
public:

	Vector3(float x, float y, float z);
	Vector3();

	void operator +=(Vector3);
	void operator -=(Vector3);
	void operator *=(Vector3);
	void operator /=(Vector3);

	void operator +=(float);
	void operator -=(float);
	void operator *=(float);
	void operator /=(float);

	Vector3 operator +(Vector3);
	Vector3 operator -(Vector3);
	
	Vector3 operator-();

	float operator *(Vector3); // ��������� ������������ �������� (Dot)
	Vector3 operator ^(Vector3); // ��������� ������������ �������� (Cross)

	Vector3 operator +(float);
	Vector3 operator -(float);
	Vector3 operator *(float);
	Vector3 operator /(float);

	void operator =(Vector3);

	void operator =(float);

	int operator ==(Vector3);
	int operator !=(Vector3);

	int operator ==(float);
	int operator !=(float);

	Vector3 Normal() const;
	void Normalize();
	float Length();
	float PlaneDistance(Vector3 vPlane, Vector3 vNormal);
public:
	float x;
	float y;
	float z;
};