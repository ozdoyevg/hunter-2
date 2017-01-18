
#include "dxsdk.h"
#include "vector3.h"
#include "vector2.h"

class CObject
{
public:
	float scale;
	Vector3 velocity;
	Vector3 rotation;
	Vector3 position;

	CObject();

	void LoadMesh(IDirect3DDevice9 *d3dDevice, char *FileName);
	void ShowMesh(IDirect3DDevice9 *d3dDevice);
	BOOL DirectionCheck(D3DVECTOR, D3DVECTOR);
private:
	ID3DXMesh *pMesh;
	D3DMATERIAL9 *pMeshMaterial;
	IDirect3DTexture9 **pMeshTextures;
	DWORD dwNum;
};

class CTexture
{
public:
	Vector2 position;
	Vector2 size;

	DWORD Color;

	CTexture();

	void InitTexture(IDirect3DDevice9 *d3dDevice);
	void LoadTexture(IDirect3DDevice9 *d3dDevice, char *FileName);
	void DrawTexture(IDirect3DDevice9 *d3dDevice);
private:
	struct GR2D_VERTEX
	{
		float x, y, z, rhw;
		D3DCOLOR color;
		FLOAT u, v;
	};

	IDirect3DTexture9 *Texture;
	IDirect3DVertexBuffer9 *VertexBuffer;
};