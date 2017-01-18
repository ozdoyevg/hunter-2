
#include "common.h"
#include "animation.h"

CSkinnedMesh::CSkinnedMesh()
{
	position = 0.0f;
	velocity = 0.0f;
	rotation = 0.0f;
	scale = 1.0f;
}

CSkinnedMesh::~CSkinnedMesh()
{
	SAFE_RELEASE(AnimController);
	ReleaseAttributeTable(FrameRoot);
}
void CSkinnedMesh::Render(IDirect3DDevice9 *d3dDevice)
{
	FrameMove(d3dDevice);
	DrawFrame(d3dDevice, FrameRoot);
}
void CSkinnedMesh::InitSkinnedMesh(IDirect3DDevice9 *d3dDevice, char *FileName)
{
	AllocMeshHierarchy Alloc(this);
	D3DXLoadMeshHierarchyFromX(
		FileName,
		D3DXMESH_MANAGED,
		d3dDevice,
		&Alloc,
		NULL,
		&FrameRoot,
		&AnimController);
	SetupBoneMatrixPointers(FrameRoot);
	D3DXFrameCalculateBoundingSphere(FrameRoot, &vObjectCenter, &fObjectRadius);
}
void CSkinnedMesh::GenerateSkinnedMesh(IDirect3DDevice9 *d3dDevice, D3DXMESHCONTAINER_DERIVED *pMeshContainer)
{
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
	pMeshContainer->pOrigMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMeshContainer->pOrigMesh->GetFVF(),
		d3dDevice, &pMeshContainer->MeshData.pMesh);
	pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->NumAttributeGroups);
	delete[] pMeshContainer->pAttributeTable;
	pMeshContainer->pAttributeTable  = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
	pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);
	if (g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
	{
		g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();
		delete[] BoneMatrices; 
		BoneMatrices  = new D3DXMATRIXA16[g_NumBoneMatricesMax];
	}
}
void CSkinnedMesh::DrawMeshContainer(IDirect3DDevice9 *d3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	pMesh = pMeshContainerBase->MeshData.pMesh;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	UINT iAttrib;
	D3DXMATRIX  Identity;
	DWORD cBones  = pMeshContainer->pSkinInfo->GetNumBones();
	DWORD iBone;
	PBYTE pbVerticesSrc;
	PBYTE pbVerticesDest;
	if (pMeshContainer->pSkinInfo != NULL)
	{
		for (iBone = 0; iBone < cBones; ++iBone)
		{
			D3DXMatrixMultiply(
					&BoneMatrices[iBone],
					&pMeshContainer->pBoneOffsetMatrices[iBone],
					pMeshContainer->ppBoneMatrixPtrs[iBone]);
		}
		D3DXMatrixIdentity(&Identity);
		d3dDevice->SetTransform(D3DTS_WORLD, &Identity);
		pMeshContainer->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&pbVerticesDest);
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(BoneMatrices, NULL, pbVerticesSrc, pbVerticesDest);
		pMeshContainer->pOrigMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
		{
			d3dDevice->SetMaterial(&(pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[iAttrib].AttribId].MatD3D));
			d3dDevice->SetTexture(0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId]);

			pMeshContainer->MeshData.pMesh->DrawSubset(pMeshContainer->pAttributeTable[iAttrib].AttribId);
		}
	}
}
void CSkinnedMesh::DrawFrame(IDirect3DDevice9 *d3dDevice, LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(d3dDevice, pMeshContainer, pFrame );
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(d3dDevice, pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(d3dDevice, pFrame->pFrameFirstChild);
	}
}

void CSkinnedMesh::FrameMove(IDirect3DDevice9* pd3dDevice)
{
	position += velocity * elapsedTime * 20;

	D3DXMATRIX mTransform;
	D3DXVECTOR3 pTranslation(position.x, position.y, position.z);
	D3DXVECTOR3 pRotationCenter(0, 0, -position.z);
	D3DXQUATERNION pRotation;
	D3DXQUATERNION pRotationX;
	D3DXQUATERNION pRotationY;

	D3DXVECTOR3 pRotY(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 pRotX(1.0f, 0.0f, 0.0f);

	D3DXQuaternionRotationAxis(&pRotationY, &pRotY, -rotation.y);
	D3DXQuaternionRotationAxis(&pRotationX, &pRotX, rotation.x);

	D3DXQuaternionMultiply(&pRotation,&pRotationX,&pRotationY);
	D3DXMatrixAffineTransformation( &mTransform, scale, &pRotationCenter, &pRotation, &pTranslation);

	pd3dDevice->SetTransform( D3DTS_WORLD, &mTransform );

	if(AnimController != NULL)
		AnimController->AdvanceTime(elapsedTime, NULL);

	UpdateFrameMatrices(FrameRoot, &mTransform);
}
void CSkinnedMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	if (pParentMatrix != NULL)
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	else pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	if (pFrame->pFrameSibling != NULL) UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	if (pFrame->pFrameFirstChild != NULL) UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
}

void CSkinnedMesh::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iBone, cBones;
	D3DXFRAME_DERIVED *pFrame;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	if (pMeshContainer->pSkinInfo != NULL)
	{
		cBones = pMeshContainer->pSkinInfo->GetNumBones();
		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		for (iBone = 0; iBone < cBones; iBone++)
		{
			pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(FrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}
}

void CSkinnedMesh::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
	if(pFrame->pMeshContainer != NULL)
	{
		SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
	}
	if(pFrame->pFrameSibling != NULL)
	{
		SetupBoneMatrixPointers(pFrame->pFrameSibling);
	}
	if(pFrame->pFrameFirstChild != NULL)
	{
		SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
	}
}

BOOL CSkinnedMesh::DirectionCheck(D3DVECTOR VERTEX1, D3DVECTOR VERTEX2)
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

	D3DXMATRIX mWorldView = matView;
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

void CSkinnedMesh::SetAnimation(UINT Index)
{
	ID3DXAnimationSet* Anim = 0;
	AnimController->GetAnimationSet(Index, &Anim);
	AnimController->SetTrackAnimationSet(0, Anim);
	AnimController->SetTrackPosition(0, 0);
	AnimController->ResetTime();
	Anim->Release();
}

void CSkinnedMesh::ReleaseAttributeTable(LPD3DXFRAME pFrameBase)
{
	D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer;
	pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pFrame->pMeshContainer;
	while( pMeshContainer != NULL )
	{
		delete[] pMeshContainer->pAttributeTable;
		pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pMeshContainer->pNextMeshContainer;
	}
	if (pFrame->pFrameSibling != NULL) ReleaseAttributeTable(pFrame->pFrameSibling);
	if (pFrame->pFrameFirstChild != NULL) ReleaseAttributeTable(pFrame->pFrameFirstChild);
}

HRESULT AllocateName( LPCTSTR Name, LPTSTR *pNewName )
{
	UINT cbLength;
	if (Name != NULL)
	{
		cbLength = lstrlen(Name) + 1;
		*pNewName = new TCHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength*sizeof(TCHAR));
	}
	else
	{
		*pNewName = NULL;
	}
	return S_OK;
}

long __stdcall AllocMeshHierarchy::CreateFrame(PCSTR Name, D3DXFRAME** ppNewFrame)
{
	HRESULT hr = S_OK;
	D3DXFRAME_DERIVED *pFrame;

	*ppNewFrame = NULL;

	pFrame = new D3DXFRAME_DERIVED;
	if (pFrame == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
		goto e_Exit;

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;
e_Exit:
	delete pFrame;
	return hr;
}

long __stdcall AllocMeshHierarchy::CreateMeshContainer(
		PCSTR Name,
		const D3DXMESHDATA* pMeshData,
		const D3DXMATERIAL* pMaterials,
		const D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		const DWORD *pAdjacency,
		ID3DXSkinInfo* pSkinInfo,
		D3DXMESHCONTAINER** ppNewMeshContainer)
{
	HRESULT hr;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
	UINT NumFaces;
	UINT iMaterial;
	UINT iBone, cBones;
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;
	LPD3DXMESH pMesh = NULL;
	*ppNewMeshContainer = NULL;
	if (pMeshData->Type != D3DXMESHTYPE_MESH)
    {
        hr = E_FAIL;
        goto e_Exit;
    }
    pMesh = pMeshData->pMesh;
    if (pMesh->GetFVF() == 0)
    {
        hr = E_FAIL;
        goto e_Exit;
    }
    pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
    if (pMeshContainer == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));
    hr = AllocateName(Name, &pMeshContainer->Name);
    if (FAILED(hr))
        goto e_Exit;        
    pMesh->GetDevice(&pd3dDevice);
    NumFaces = pMesh->GetNumFaces();
    if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
    {
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
        hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                    pMesh->GetFVF() | D3DFVF_NORMAL, 
                                    pd3dDevice, &pMeshContainer->MeshData.pMesh );
        if (FAILED(hr))
            goto e_Exit;
        pMesh = pMeshContainer->MeshData.pMesh;
        D3DXComputeNormals( pMesh, NULL );
    }
    else
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
        pMesh->AddRef();
    }
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[NumFaces*3];
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
    if (NumMaterials > 0)            
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);
        for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
        {
            if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
            {
                if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, pMeshContainer->pMaterials[iMaterial].pTextureFilename,
                                                        &pMeshContainer->ppTextures[iMaterial] ) ) )
                    pMeshContainer->ppTextures[iMaterial] = NULL;
					pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
					pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
					pMeshContainer->pMaterials[iMaterial].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
            }
        }
    }
    else
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }
    if (pSkinInfo != NULL)
    {
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        pMeshContainer->pOrigMesh = pMesh;
        pMesh->AddRef();
        cBones = pSkinInfo->GetNumBones();
        pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if (pMeshContainer->pBoneOffsetMatrices == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}
		for (iBone = 0; iBone < cBones; iBone++)
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
		}
		SkinMesh->GenerateSkinnedMesh(pd3dDevice, pMeshContainer);
		if (FAILED(hr))
			goto e_Exit;
	}
	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;
e_Exit:
	SAFE_RELEASE(pd3dDevice);
	if (pMeshContainer != NULL)
	{
		DestroyMeshContainer(pMeshContainer);
	}
	return hr;
}

long __stdcall AllocMeshHierarchy::DestroyFrame(D3DXFRAME* pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

long __stdcall AllocMeshHierarchy::DestroyMeshContainer(D3DXMESHCONTAINER* pMeshContainerBase)
{
	UINT iMaterial;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	SAFE_DELETE_ARRAY( pMeshContainer->Name );
	SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
	SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
	pMeshContainer->pBoneOffsetMatrices=NULL;
	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
			SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial + pMeshContainer->NumMaterials] );
		}
	}
	SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
	SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs );
	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );
	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	SAFE_RELEASE( pMeshContainer->pSkinInfo );
	SAFE_RELEASE( pMeshContainer->pOrigMesh );
	SAFE_DELETE( pMeshContainer );
	return S_OK;
}