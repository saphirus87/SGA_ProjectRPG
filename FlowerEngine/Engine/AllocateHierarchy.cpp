#include "stdafx.h"
#include "AllocateHierarchy.h"


AllocateHierarchy::AllocateHierarchy(CString szFolderPath) : 
	m_szFolderPath(szFolderPath),
	NumBoneMatricesMax(0),
	pDevice9(GetD3D9Device())
{
}


AllocateHierarchy::~AllocateHierarchy()
{
}

HRESULT AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	// 하나의 프레임 생성
	Frame* pFrame = new Frame;

	// 프레임 이름 지정
	pFrame->Name = NULL;
	if (Name != NULL)
	{
		pFrame->Name = new char[strlen(Name) + 1];
		strcpy_s(pFrame->Name, (strlen(Name) + 1), Name);
	}

	// 행렬 초기화
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTM);

	// 포인터 초기화
	pFrame->pMeshContainer = NULL;
	pFrame->pFrameFirstChild = NULL;
	pFrame->pFrameSibling = NULL;

	// 생성된 프레임 값 할당
	*ppNewFrame = pFrame;

	return S_OK;
}

HRESULT AllocateHierarchy::CreateMeshContainer(LPCSTR Name, const D3DXMESHDATA * pMeshData, const D3DXMATERIAL * pMaterials, const D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, const DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	// 매쉬 에러 체크
	Mesh pMesh = pMeshData->pMesh;
	assert(pMeshData->Type == D3DXMESHTYPE_MESH && _T("D3DXMESHTYPE_MESH 타입만 사용 가능"));
	assert(pMesh->GetFVF() > 0 && _T("FVF Flag 가 지정되지 않음"));

	// 매쉬 컨테이너 생성
	MeshContainer* pMeshContainer = new MeshContainer;
	// 매쉬 컨테이너 이름 지정
	if (Name != NULL)
	{
		pMeshContainer->Name = new char[strlen(Name) + 1];
		strcpy_s(pMeshContainer->Name, (strlen(Name) + 1), Name);
	}

	// 매쉬에 노멀값이 없을 경우 추가
	if ((pMesh->GetFVF() & D3DFVF_NORMAL) == false)
	{
		// MeshFVF를 복제
		pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL, pDevice9, &pMesh);
		// 이 매쉬의 노멀값 계산
		D3DXComputeNormals(pMesh, NULL);
	}

	// 인접 폴리곤 메모리 할당과 지정
	UINT NumFaces = pMesh->GetNumFaces();
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);

	// 재질 갯수 지정
	pMeshContainer->NumMaterials = max(1, NumMaterials);

	// 재질이 있다면 지정
	if (NumMaterials > 0)
	{
		for (int i = 0; i < NumMaterials; ++i)
		{
			// 여기에 범프맵 같은것도 추가해야 할듯..
			MTLTEX* pMtlTex = new MTLTEX;
			pMtlTex->material = pMaterials[i].MatD3D;
			pMtlTex->pTexture = Assets::GetTexture(m_szFolderPath + pMaterials[i].pTextureFilename);
			pMeshContainer->vecMtlTex.push_back(pMtlTex);
		}
	}
	else // 재질이 없다면 기본
		pMeshContainer->vecMtlTex.push_back(new MTLTEX);

	// 매쉬 컨테이너에 매쉬 지정
	pMeshContainer->MeshData.pMesh = pMesh;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	pMeshContainer->pOrigMesh = pMesh; // 원본 매쉬 지정
	// 이 DX 매쉬의 레퍼런스 증가
	SAFE_ADDREF(pMesh);

	// DX 스킨 정보가 있다면 지정
	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		// 스킨 정보의 레퍼런스 증가
		SAFE_ADDREF(pSkinInfo);
		// 이 매쉬에 영향을 주는 뼈대의 갯수
		DWORD numBones = pSkinInfo->GetNumBones();
		pMeshContainer->ppBoneMatrixPtrs = new Matrix4x4*[numBones];
		pMeshContainer->pBoneOffsetMatrices = new Matrix4x4[numBones];

		// 매쉬 컨테이너의 뼈대 행렬들을 스키닝 정보에서 가져와서 지정
		for (DWORD i = 0; i < numBones; ++i)
			pMeshContainer->pBoneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));

		GenerateSkinnedMesh(pMeshContainer);
	}

	// 생성된 매쉬 컨테이너 지정하여 반환
	*ppNewMeshContainer = pMeshContainer;

	return S_OK;
}

void AllocateHierarchy::GenerateSkinnedMesh(MeshContainer * pMeshContainer)
{
	D3DCAPS9 d3dCaps;
	pDevice9->GetDeviceCaps(&d3dCaps);

	// 팔레트 크기 가져 오기
	// 처음 9 개의 상수가 다른 데이터에 사용됩니다. 각 4x3 행렬은 3 개의 상수를 차지합니다.
	// (96 - 9) / 3 즉 최대 상수 - 사용 된 상수

	// 매쉬 컨테이너의 뼈 갯수 지정
	UINT MaxMatrices = 26;
	pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	// 인덱싱 블렌딩된 매쉬로 변환
	pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh(
		pMeshContainer->pOrigMesh,
		D3DXMESHOPT_VERTEXCACHE | D3DXMESH_MANAGED,
		pMeshContainer->NumPaletteEntries,
		pMeshContainer->pAdjacency,
		NULL, NULL, NULL,
		&pMeshContainer->NumInfl,
		&pMeshContainer->NumAttributeGroups,
		&pMeshContainer->pBoneCombinationBuf,
		&pMeshContainer->MeshData.pMesh);

	// 매쉬 컨테이너의 원본 매쉬 지정
	pMeshContainer->pOrigMesh = pMeshContainer->MeshData.pMesh;

	// FVF는 선언 자와 일치해야합니다. (FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline)
	DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;

	// 변환된 매쉬가 기존 FVF와 같지 않다면
	if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
	{
		// MeshFVF를 복사해 옵니다.
		Mesh pMesh = NULL;
		pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, pDevice9, &pMesh);
		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		// 컨테이너의 매쉬를 복제된 매쉬로 지정
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->pOrigMesh = pMesh;
	}

	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);

	// 버텍스 쉐이더가 UBYTE4를 D3DCOLOR로 해석 할 것으로 예상되므로 타입을 업데이트하십시오 (the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type)
	// 참고 : UBYTE4 데이터를 float로 변환 한 다음 D3DCOLOR로 변환하는 CloneMesh로는이 작업을 수행 할 수 없습니다. (NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR)
	//이 작업은 "캐스트"작업에 가깝습니다. (this is more of a "cast" operation)
	pDeclCur = pDecl;
	while (pDeclCur->Stream != 0xff)
	{
		if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		++pDeclCur;
	}

	// 정점 선언 정보 업데이트
	pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);

	// 다른 메쉬가 같은 크기 이상을 할당하지 않은 경우에만 뼈대 행렬에 버퍼를 할당합니다. (allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger)
	if (NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
		NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();
}

HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	// 뼈대들 모두 메모리 해제
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

HRESULT AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	// 메쉬 컨테이너 메모리 해제
	MeshContainer* pMeshContainer = (MeshContainer*)pMeshContainerToFree;
	if (pMeshContainer == NULL)
		return E_FAIL;

	SAFE_RELEASE(pMeshContainer->pOrigMesh);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);

	for (auto p : pMeshContainer->vecMtlTex)
		SAFE_DELETE(p);

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

	SAFE_DELETE(pMeshContainerToFree);

	return S_OK;
}