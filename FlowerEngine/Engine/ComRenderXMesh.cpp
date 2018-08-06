#include "stdafx.h"
#include "ComRenderXMesh.h"


ComRenderXMesh::ComRenderXMesh(CString szName) : 
	Component(szName),
	m_pMesh(NULL),
	m_iNumMaterials(0),
	m_pEffect(NULL)
{
	pDevice9 = GetD3D9Device();
	D3DXMatrixIdentity(&matFrame);
	D3DXMatrixIdentity(&matParent);
}


ComRenderXMesh::~ComRenderXMesh()
{
	SAFE_RELEASE(m_pMesh);
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
	// 현재 행렬 * Combined Matrix * parent Obj Matrix
	// 현재 행렬은 즉 scale, rot, transfomation
	//Matrix4x4 matFinal = /*gameObject->transform->GetWorldMatrix() **/ matFrame * matParent;

	Matrix4x4 matRotY;
	D3DXMatrixIdentity(&matRotY);
	float fRotY = gameObject->transform->GetRotation().y;
	float fDegreeY = D3DXToDegree(fRotY);
	D3DXMatrixRotationY(&matRotY, fRotY);

	// matFrame의 스케일 값이 1 이하인듯
	matFrame._11 = 1.0f;
	matFrame._22 = 1.0f;
	matFrame._33 = 1.0f;
	Matrix4x4 matFinal = matRotY * matFrame * matParent;
	
	m_pEffect->SetMatrix("gWorldMatrix", &matFinal);
	m_pEffect->SetMatrix("gViewMatrix", &Camera::GetInstance()->GetViewMatrix());
	m_pEffect->SetMatrix("gProjMatrix", &Camera::GetInstance()->GetProjMatrix());

	
	UINT pass;
	m_pEffect->Begin(&pass, NULL);
	m_pEffect->BeginPass(0);

	//pDevice9->SetTransform(D3DTS_WORLD, &matFinal);
	for (DWORD i = 0; i < m_iNumMaterials; ++i)
	{
		m_pEffect->SetTexture("DiffuseMap_Tex", m_vecMtrl[i].pTexture);
		m_pEffect->CommitChanges();
		// 셰이더 필요
		//pDevice9->SetMaterial(&m_vecMtrl[i].material);
		//pDevice9->SetTexture(0, m_vecMtrl[i].pTexture);
		m_pMesh->DrawSubset(i);
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
