
#include "common.h"
#include "object.h"

CObject::CObject()
{
	position = 0;
	velocity = 0;
	rotation = 0;
	scale = 1.0f;
};
void CObject::LoadMesh(IDirect3DDevice9 *d3dDevice, char *szFile)
{
	ID3DXBuffer *pBuffer;
    D3DXLoadMeshFromX(szFile, D3DXMESH_SYSTEMMEM, d3dDevice, NULL, &pBuffer, NULL, &dwNum, &pMesh);

    pMeshMaterial = new D3DMATERIAL9[dwNum];
    pMeshTextures = new LPDIRECT3DTEXTURE9[dwNum];

    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pBuffer->GetBufferPointer();
    for (DWORD i = 0; i < dwNum; i++)
    {
        pMeshMaterial[i] = d3dxMaterials[i].MatD3D;
        pMeshMaterial[i].Ambient = pMeshMaterial[i].Diffuse;

        if(FAILED(D3DXCreateTextureFromFile(d3dDevice,
		d3dxMaterials[i].pTextureFilename, &pMeshTextures[i])))
		{
			pMeshTextures[i] = NULL;
		}
	}
	pBuffer->Release();
};

void CObject::ShowMesh(IDirect3DDevice9 *d3dDevice)
{
	position += velocity;

	D3DXMATRIX mTransform;

	D3DXMatrixTranslation(&mTransform, position.x, position.y, position.z);
	d3dDevice -> SetTransform(D3DTS_WORLD, &mTransform);

    for (DWORD b = 0; b < dwNum; b++)
    {
        d3dDevice->SetMaterial(&pMeshMaterial[b]);
        d3dDevice->SetTexture(0, pMeshTextures[b]);
    }
	pMesh->DrawSubset(0);
}

BOOL CObject::DirectionCheck(D3DVECTOR VERTEX1, D3DVECTOR VERTEX2)
{
	BOOL bHit;
	D3DXVECTOR3 vPickRayDir;
	D3DXVECTOR3 vPickRayOrig;

	D3DXMATRIXA16 matWorld;
	D3DXMatrixTranslation(&matWorld, position.x, position.y, position.z);

	D3DXMATRIXA16 matView;
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 vLookAt = VERTEX2;
	D3DXVECTOR3 vEyePos = VERTEX1;
	D3DXMatrixLookAtLH( &matView, &vEyePos, &vLookAt, &vUpVec );
 
	D3DXVECTOR3 v;
	v.x = 0;
	v.y = 0;
	v.z =  1.0f;

	D3DXMATRIX mWorldView = matWorld * matView;
	D3DXMATRIX m;
	D3DXMatrixInverse( &m, NULL, &mWorldView );

	vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;

	D3DXIntersect(pMesh, &vPickRayOrig, &vPickRayDir, &bHit,0,0,0,0,0,0);
	return bHit;
}

CTexture::CTexture()
{
	position = 0;
	size = 100;
	Color = 0xffffffff;
}

void CTexture::InitTexture(IDirect3DDevice9 *d3dDevice)
{
    GR2D_VERTEX verticies[] =
    {
		{ position.x,			position.y, 0.0f, 1.0f, Color, 0, 0 },
		{ position.x + size.x,	position.y, 0.0f, 1.0f, Color, 1, 0 },
		{ position.x,			position.y + size.y, 0.0f, 1.0f, Color, 0, 1 },
		{ position.x + size.x,	position.y + size.y, 0.0f, 1.0f, Color, 1, 1 },
		{ position.x,			position.y + size.y, 0.0f, 1.0f, Color, 0, 1 },
		{ position.x + size.x,	position.y, 0.0f, 1.0f, Color, 1, 0 },

    };

	d3dDevice->CreateVertexBuffer(
		6*sizeof(verticies),
		0, 
		D3DFVF_XYZRHW | D3DFVF_DIFFUSE|D3DFVF_TEX1, 
		D3DPOOL_DEFAULT, 
		&VertexBuffer, 
		NULL );

	VOID* pVertices;
	VertexBuffer->Lock(0, sizeof(verticies), (void**)&pVertices, 0 );
	memcpy(pVertices, verticies, sizeof(verticies));
	VertexBuffer->Unlock();

}
void CTexture::LoadTexture(IDirect3DDevice9 *d3dDevice, char *FileName)
{
	D3DXCreateTextureFromFile(d3dDevice, FileName, &Texture);
}
void CTexture::DrawTexture(IDirect3DDevice9 *d3dDevice)
{
	d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE|D3DFVF_TEX1);
	d3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dDevice->SetTexture(0, Texture);
	d3dDevice->SetStreamSource(0, VertexBuffer, 0, sizeof(GR2D_VERTEX));
	d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	d3dDevice->SetRenderState(D3DRS_LIGHTING, true);
}