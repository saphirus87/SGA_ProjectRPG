#include "stdafx.h"
#include "ComRenderXMesh.h"


ComRenderXMesh::ComRenderXMesh(CString szName) : 
	Component(szName),
	m_pMesh(NULL),
	m_iNumMaterials(0),
	m_pEffect(NULL),
	IsMirrored(false)
{
	pDevice9 = GetD3D9Device();
	D3DXMatrixIdentity(&m_matFrame);
	D3DXMatrixIdentity(&m_matParent);
}


ComRenderXMesh::~ComRenderXMesh()
{
	SAFE_RELEASE(m_pMesh);
	m_vecMtrl.clear();
}

void ComRenderXMesh::Awake()
{
	m_pEffect = Shaders::GetInstance()->GetShader(SHADER_PATH + "/ShaderPT.fx");
}

void ComRenderXMesh::Update()
{
}

void ComRenderXMesh::Render()
{
	// 현재 행렬 * Combined Matrix * parent Obj Matrix (현재 행렬은 즉 scale, rot, transfomation)
	Matrix4x4 matFinal = gameObject->transform->GetWorldMatrix() * m_matFrame * m_matParent;
	
	m_pEffect->SetMatrix("gWorldMatrix", &matFinal);
	m_pEffect->SetMatrix("gViewMatrix", &Camera::GetInstance()->GetViewMatrix());
	m_pEffect->SetMatrix("gProjMatrix", &Camera::GetInstance()->GetProjMatrix());
		
	UINT pass;
	m_pEffect->Begin(&pass, NULL);
	m_pEffect->BeginPass(0);

	for (DWORD i = 0; i < m_iNumMaterials; ++i)
	{
		m_pEffect->SetTexture("DiffuseMap_Tex", m_vecMtrl[i].pTexture);
		m_pEffect->CommitChanges();
		if (IsMirrored)
			pDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		else
			pDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pMesh->DrawSubset(i);
		pDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	m_pEffect->EndPass();
	m_pEffect->End();
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

void ComRenderXMesh::Clone(ComRenderXMesh* pComRenderXMesh)
{
	Mesh pMesh = pComRenderXMesh->m_pMesh;
	pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF(), pDevice9, &m_pMesh);
	m_iNumMaterials = pComRenderXMesh->m_iNumMaterials;
	m_vecMtrl.resize(pComRenderXMesh->m_iNumMaterials);
	for (DWORD i = 0; i < m_iNumMaterials; ++i)
		m_vecMtrl[i] = pComRenderXMesh->m_vecMtrl[i];
}

void ComRenderXMesh::SetFrameMatrix(Matrix4x4 * pMatFrame, Matrix4x4 * pMatParent)
{
	m_matFrame = *pMatFrame;
	m_matParent = *pMatParent;
}
