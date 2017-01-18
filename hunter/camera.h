
#pragma once

#include "dxsdk.h"
#include "vector2.h"

class CCamera
{
public:
	D3DVECTOR position;
	D3DVECTOR direction;

	void Initialize(D3DVECTOR position, D3DVECTOR direction);
	void SetRotation(Vector2 rotation);
private:
	D3DVECTOR firstDirection;
};