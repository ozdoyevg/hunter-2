
#include "common.h"
#include "camera.h"

void CCamera::Initialize(D3DVECTOR position, D3DVECTOR direction)
{
	CCamera::position = position;
	CCamera::direction = direction;
	firstDirection = direction;
}

void CCamera::SetRotation(Vector2 rotation)
{
	float tempZ = 0;

	float fSinX = sinf(rotation.y);
	float fCosX = cosf(rotation.y);
	float fSinY = sinf(rotation.x);
	float fCosY = cosf(rotation.x);

	direction.y = position.y + (firstDirection.y - position.y) * fCosY - (firstDirection.z - position.z) * fSinY;
		  tempZ = position.z + (firstDirection.y - position.y) * fSinY + (firstDirection.z - position.z) * fCosY;
	direction.x = position.x + (firstDirection.x - position.x) * fCosX - (tempZ - position.z) * fSinX;
	direction.z = position.z + (firstDirection.x - position.x) * fSinX + (tempZ - position.z) * fCosX;
}