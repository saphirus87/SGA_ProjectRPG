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
	// �ϳ��� ������ ����
	Frame* pFrame = new Frame;

	// ������ �̸� ����
	pFrame->Name = NULL;
	if (Name != NULL)
	{
		pFrame->Name = new char[strlen(Name) + 1];
		strcpy_s(pFrame->Name, (strlen(Name) + 1), Name);
	}

	// ��� �ʱ�ȭ
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTM);

	// ������ �ʱ�ȭ
	pFrame->pMeshContainer = NULL;
	pFrame->pFrameFirstChild = NULL;
	pFrame->pFrameSibling = NULL;

	// ������ ������ �� �Ҵ�
	*ppNewFrame = pFrame;

	return S_OK;
}

HRESULT AllocateHierarchy::CreateMeshContainer(LPCSTR Name, const D3DXMESHDATA * pMeshData, const D3DXMATERIAL * pMaterials, const D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, const DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	// �Ž� ���� üũ
	Mesh pMesh = pMeshData->pMesh;
	assert(pMeshData->Type == D3DXMESHTYPE_MESH && _T("D3DXMESHTYPE_MESH Ÿ�Ը� ��� ����"));
	assert(pMesh->GetFVF() > 0 && _T("FVF Flag �� �������� ����"));

	// �Ž� �����̳� ����
	MeshContainer* pMeshContainer = new MeshContainer;
	// �Ž� �����̳� �̸� ����
	if (Name != NULL)
	{
		pMeshContainer->Name = new char[strlen(Name) + 1];
		strcpy_s(pMeshContainer->Name, (strlen(Name) + 1), Name);
	}

	// �Ž��� ��ְ��� ���� ��� �߰�
	if ((pMesh->GetFVF() & D3DFVF_NORMAL) == false)
	{
		// MeshFVF�� ����
		pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL, pDevice9, &pMesh);
		// �� �Ž��� ��ְ� ���
		D3DXComputeNormals(pMesh, NULL);
	}

	// ���� ������ �޸� �Ҵ�� ����
	UINT NumFaces = pMesh->GetNumFaces();
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);

	// ���� ���� ����
	pMeshContainer->NumMaterials = max(1, NumMaterials);

	// ������ �ִٸ� ����
	if (NumMaterials > 0)
	{
		for (int i = 0; i < NumMaterials; ++i)
		{
			// ���⿡ ������ �����͵� �߰��ؾ� �ҵ�..
			MTLTEX* pMtlTex = new MTLTEX;
			pMtlTex->material = pMaterials[i].MatD3D;
			pMtlTex->pTexture = Assets::GetTexture(m_szFolderPath + pMaterials[i].pTextureFilename);
			pMeshContainer->vecMtlTex.push_back(pMtlTex);
		}
	}
	else // ������ ���ٸ� �⺻
		pMeshContainer->vecMtlTex.push_back(new MTLTEX);

	// �Ž� �����̳ʿ� �Ž� ����
	pMeshContainer->MeshData.pMesh = pMesh;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	pMeshContainer->pOrigMesh = pMesh; // ���� �Ž� ����
	// �� DX �Ž��� ���۷��� ����
	SAFE_ADDREF(pMesh);

	// DX ��Ų ������ �ִٸ� ����
	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		// ��Ų ������ ���۷��� ����
		SAFE_ADDREF(pSkinInfo);
		// �� �Ž��� ������ �ִ� ������ ����
		DWORD numBones = pSkinInfo->GetNumBones();
		pMeshContainer->ppBoneMatrixPtrs = new Matrix4x4*[numBones];
		pMeshContainer->pBoneOffsetMatrices = new Matrix4x4[numBones];

		// �Ž� �����̳��� ���� ��ĵ��� ��Ű�� �������� �����ͼ� ����
		for (DWORD i = 0; i < numBones; ++i)
			pMeshContainer->pBoneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));

		GenerateSkinnedMesh(pMeshContainer);
	}

	// ������ �Ž� �����̳� �����Ͽ� ��ȯ
	*ppNewMeshContainer = pMeshContainer;

	return S_OK;
}

void AllocateHierarchy::GenerateSkinnedMesh(MeshContainer * pMeshContainer)
{
	D3DCAPS9 d3dCaps;
	pDevice9->GetDeviceCaps(&d3dCaps);

	// �ȷ�Ʈ ũ�� ���� ����
	// ó�� 9 ���� ����� �ٸ� �����Ϳ� ���˴ϴ�. �� 4x3 ����� 3 ���� ����� �����մϴ�.
	// (96 - 9) / 3 �� �ִ� ��� - ��� �� ���

	// �Ž� �����̳��� �� ���� ����
	UINT MaxMatrices = 26;
	pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	// �ε��� ������ �Ž��� ��ȯ
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

	// �Ž� �����̳��� ���� �Ž� ����
	pMeshContainer->pOrigMesh = pMeshContainer->MeshData.pMesh;

	// FVF�� ���� �ڿ� ��ġ�ؾ��մϴ�. (FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline)
	DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;

	// ��ȯ�� �Ž��� ���� FVF�� ���� �ʴٸ�
	if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
	{
		// MeshFVF�� ������ �ɴϴ�.
		Mesh pMesh = NULL;
		pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, pDevice9, &pMesh);
		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		// �����̳��� �Ž��� ������ �Ž��� ����
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->pOrigMesh = pMesh;
	}

	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);

	// ���ؽ� ���̴��� UBYTE4�� D3DCOLOR�� �ؼ� �� ������ ����ǹǷ� Ÿ���� ������Ʈ�Ͻʽÿ� (the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type)
	// ���� : UBYTE4 �����͸� float�� ��ȯ �� ���� D3DCOLOR�� ��ȯ�ϴ� CloneMesh�δ��� �۾��� ���� �� �� �����ϴ�. (NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR)
	//�� �۾��� "ĳ��Ʈ"�۾��� �������ϴ�. (this is more of a "cast" operation)
	pDeclCur = pDecl;
	while (pDeclCur->Stream != 0xff)
	{
		if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		++pDeclCur;
	}

	// ���� ���� ���� ������Ʈ
	pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);

	// �ٸ� �޽��� ���� ũ�� �̻��� �Ҵ����� ���� ��쿡�� ���� ��Ŀ� ���۸� �Ҵ��մϴ�. (allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger)
	if (NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
		NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();
}

HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	// ����� ��� �޸� ����
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

HRESULT AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	// �޽� �����̳� �޸� ����
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