#include "stdafx.h"
#include "ComEquipment.h"
#include "ComObjMap.h"
#include "ItemInfo.h"

ComEquipment::ComEquipment(CString szName) : 
	Component(szName),
	m_pMesh(NULL),
	m_iNumMaterials(0),
	m_pEffect(NULL),
	IsMirrored(false),
	IsEquiped(false),
	IsDropped(false),
	pItemInfo(NULL)
{
	pDevice9 = GetD3D9Device();
	D3DXMatrixIdentity(&m_matFinal);
	D3DXMatrixIdentity(&m_matFrame);
	D3DXMatrixIdentity(&m_matParent);
}


ComEquipment::~ComEquipment()
{
	SAFE_DELETE(pItemInfo);
	SAFE_RELEASE(m_pMesh);
	m_vecMtrl.clear();
}

void ComEquipment::Awake()
{
	m_pEffect = Shaders::GetInstance()->GetShader(SHADER_PATH + "/ShaderPT.fx");
}

void ComEquipment::Update()
{
	if (IsDropped == false)
	{
		GameObject* pObjMap = GameObject::Find("ObjMap");
		if (pObjMap != NULL)
		{
			ComObjMap* pMap = (ComObjMap*)pObjMap->GetComponent("ComObjMap");
			Vector3 pos = gameObject->transform->GetPosition();
			float fHeight = 0.f;
			if (pMap != NULL && pMap->GetHeight(fHeight, pos) == true)
			{
				pos.y = fHeight + 0.2f;
				gameObject->transform->SetPosition(pos);
				IsDropped = true;
			}
		}
	}
}

void ComEquipment::Render()
{
	// 장착이 안되었을 경우 뼈대 행렬을 곱하지 않는다. 그러므로 크기는 다시 1이된다.
	// 지형 위에서 렌더링
	if (IsEquiped == false)
	{
		m_matFinal = gameObject->transform->GetWorldMatrix();

		RenderShader();
	}
}

void ComEquipment::Load(CString szFolderPath, CString szFileName)
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

void ComEquipment::Clone(ComEquipment* pComRenderEquipment)
{
	Mesh pMesh = pComRenderEquipment->m_pMesh;
	pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF(), pDevice9, &m_pMesh);
	m_iNumMaterials = pComRenderEquipment->m_iNumMaterials;
	m_vecMtrl.resize(pComRenderEquipment->m_iNumMaterials);
	for (DWORD i = 0; i < m_iNumMaterials; ++i)
		m_vecMtrl[i] = pComRenderEquipment->m_vecMtrl[i];
}

void ComEquipment::Render(Matrix4x4 * pMatFrame, Matrix4x4 * pMatParent)
{
	m_matFrame = *pMatFrame;
	m_matParent = *pMatParent;

	// 현재 행렬 * Combined Matrix * parent Obj Matrix (현재 행렬은 즉 scale, rot, transfomation)
	m_matFinal = gameObject->transform->GetWorldMatrix() * m_matFrame * m_matParent;

	RenderShader();
}

void ComEquipment::ChangeTexture(int iIndex, CString szTextureName)
{
	m_vecMtrl[iIndex].pTexture = Assets::GetTexture(szTextureName);
}

void ComEquipment::RenderShader()
{
	m_pEffect->SetMatrix("gWorldMatrix", &m_matFinal);
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
	}

	pDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pEffect->EndPass();
	m_pEffect->End();
}
