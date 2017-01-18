
#pragma once

#include <math.h>

class CPoint
{ 
public:
	int x;
	int y;

public:
	CPoint(int x, int y)
	{
		CPoint::x = x;
		CPoint::y = y;
	}

	CPoint(void)
	{
		x = 0;
		y = 0;
	}

	void operator +=(CPoint inPoint)
	{
		x += inPoint.x;
		y += inPoint.y;
	}

	void operator -=(CPoint inPoint)
	{
		x -= inPoint.x;
		y -= inPoint.y;
	}

	void operator +=(int value)
	{
		x += value;
		y += value;
	}

	void operator -=(int value)
	{
		x -= value;
		y -= value;
	}

	void operator *=(int value)
	{
		x *= value;
		y *= value;
	}

	void operator /=(int value)
	{
		x /= value;
		y /= value;
	}

	CPoint operator +(CPoint inPoint)
	{
		return CPoint(x + inPoint.x, y + inPoint.y);
	}

	CPoint operator -(CPoint inPoint)
	{
		return CPoint(x - inPoint.x, y - inPoint.y);
	}

	CPoint operator +(int value)
	{
		return CPoint(x + value, y + value);
	}

	CPoint operator -(int value)
	{
		return CPoint(x - value, y - value);
	}

	CPoint operator *(int value)
	{
		return CPoint(x * value, y * value);
	}

	CPoint operator /(int value)
	{
		return CPoint(x / value, y / value);
	}

	void operator =(CPoint inPoint)
	{
		x = inPoint.x;
		y = inPoint.y;
	}

	void operator =(int value)
	{
		x = value;
		y = value;
	}

	int operator ==(CPoint inPoint)
	{
		if((inPoint.x == x) && (inPoint.y == y)) return (1);
		else return (0);
	}
	int operator !=(CPoint inPoint)
	{
		if((inPoint.x != x) || (inPoint.y != y)) return (1);
		else return (0);
	}

	int operator ==(int value)
	{
		if((value == x) && (value == y)) return (1);
		else return (0);
	}

	int operator !=(int value)
	{
		if((value != x) || (value != y)) return (1);
		else return (0);
	}
};