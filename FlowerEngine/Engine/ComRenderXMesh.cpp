#include "stdafx.h"
#include "ComRenderXMesh.h"


ComRenderXMesh::ComRenderXMesh(CString szName) : 
	Component(szName),
	m_pMesh(NULL),
	m_iNumMaterials(0)
{
	pDevice9 = GetD3D9Device();
	D3DXMatrixIdentity(&matWorld);
}


ComRenderXMesh::~ComRenderXMesh()
{
	SAFE_RELEASE(m_pMesh);
}

void ComRenderXMesh::Awake()
{
}

void ComRenderXMesh::Update()
{
}

void ComRenderXMesh::Render()
{
	//Matrix4x4 matFinal = matWorld * gameObject->transform->GetWorldMatrix();
	pDevice9->SetTransform(D3DTS_WORLD, &gameObject->transform->GetWorldMatrix());
	for (DWORD i = 0; i < m_iNumMaterials; ++i)
	{
		// 셰이더 필요
		pDevice9->SetMaterial(&m_vecMtrl[i].material);
		pDevice9->SetTexture(0, m_vecMtrl[i].pTexture);
		m_pMesh->DrawSubset(i);
	}
}

void ComRenderXMesh::Load(CString szFolderPath, CString szFileName)
{
	LPD3DXBUFFER pBuffer;

	if (FAILED(D3DXLoadMeshFromX(szFolderPath + szFileName, D3DXMESH_MANAGED, pDevice9, NULL, &pBuffer, NULL, &m_iNumMaterials, &m_pMesh)))
	{
		MessageBox(NULL, L"메쉬 읽기 실패", L"Error", MB_OK);
	}

	D3DXMATERIAL* materials = (D3DXMATERIAL*)pBuffer->GetBufferPointer();

	m_vecMtrl.resize(m_iNumMaterials);
	for (DWORD i = 0; i < m_iNumMaterials; ++i)
	{
		MTLTEX mtrl;
		mtrl.id = i;
		mtrl.material = materials[i].MatD3D;
		if (materials[i].pTextureFilename != NULL && lstrlenA(materials[i].pTextureFilename) > 0)
			mtrl.pTexture = Assets::GetTexture(CString(szFolderPath + materials[i].pTextureFilename));
		m_vecMtrl[i] = mtrl;
	}

	pBuffer->Release();
}
