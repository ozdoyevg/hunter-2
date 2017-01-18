
#pragma once

#include <malloc.h>

#include "dxsdk.h"
#include "vector3.h"

struct D3DXFRAME_DERIVED: public D3DXFRAME
{
    D3DXMATRIXA16        CombinedTransformationMatrix;
};

struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
    LPDIRECT3DTEXTURE9*  ppTextures;

    LPD3DXMESH           pOrigMesh;
    LPD3DXATTRIBUTERANGE pAttributeTable;
    DWORD                NumAttributeGroups;
    DWORD                NumInfl;
    LPD3DXBUFFER         pBoneCombinationBuf;
    D3DXMATRIX**         ppBoneMatrixPtrs;
    D3DXMATRIX*          pBoneOffsetMatrices;
    DWORD                NumPaletteEntries;
    bool                 UseSoftwareVP;
    DWORD                iAttributeSW;
};

class CSkinnedMesh
{
public:
	CSkinnedMesh();
	~CSkinnedMesh();

	void DrawFrame(IDirect3DDevice9 *d3dDevice, LPD3DXFRAME pFrame);
	void InitSkinnedMesh(IDirect3DDevice9 *d3dDevice, char *FileName);
	void GenerateSkinnedMesh(IDirect3DDevice9 *d3dDevice, D3DXMESHCONTAINER_DERIVED *pMeshContainer);
	void Render(IDirect3DDevice9 *d3dDevice);
	void SetAnimation(UINT Index);
	BOOL DirectionCheck(D3DVECTOR VERTEX1, D3DVECTOR VERTEX2);

	float scale;
	Vector3 velocity;
	Vector3 rotation;
	Vector3 position;

	float elapsedTime;
private:
	void SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);
	void SetupBoneMatrixPointers(LPD3DXFRAME pFrame);
	void DrawMeshContainer(IDirect3DDevice9 *d3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	void ReleaseAttributeTable(LPD3DXFRAME pFrameBase);
	void FrameMove( IDirect3DDevice9* pd3dDevice);
	void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );

	LPD3DXFRAME FrameRoot;
	ID3DXAnimationController* AnimController;
	D3DXVECTOR3 vObjectCenter;
	float fObjectRadius;
	D3DXMATRIXA16* BoneMatrices;
	UINT g_NumBoneMatricesMax;
	ID3DXMesh *pMesh;
};


__declspec(align(16)) class AllocMeshHierarchy : public ID3DXAllocateHierarchy
{
public:
	virtual long __stdcall CreateFrame(PCSTR Name, D3DXFRAME** ppNewFrame);
	virtual long __stdcall CreateMeshContainer(
		PCSTR Name,
		const D3DXMESHDATA* pMeshData,
		const D3DXMATERIAL* pMaterials,
		const D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		const DWORD *pAdjacency,
		ID3DXSkinInfo* pSkinInfo,
        D3DXMESHCONTAINER** ppNewMeshContainer);

	virtual long __stdcall DestroyFrame(D3DXFRAME* pFrameToFree);
	virtual long __stdcall DestroyMeshContainer(D3DXMESHCONTAINER* pMeshContainerBase);
	AllocMeshHierarchy(CSkinnedMesh *pSkinMesh) :SkinMesh(pSkinMesh) {}
public:
	CSkinnedMesh *SkinMesh;
};