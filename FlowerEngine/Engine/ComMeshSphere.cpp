#include "stdafx.h"
#include "ComMeshSphere.h"


ComMeshSphere::ComMeshSphere(CString szName) : 
	Component(szName),
	m_pMesh(NULL),
	m_pEffect(NULL)
{
	pDevice9 = GetD3D9Device();
}

ComMeshSphere::~ComMeshSphere()
{
	SAFE_RELEASE(m_pMesh);
}

void ComMeshSphere::Awake()
{
	/*m_pEffect = DX9::GetInstance()->GetShader("Resources/shader/TextureMapping.fx");
	D3DXLoadMeshFromX(_T("Resources/shaderResource/Sphere.x"), D3DXMESH_MANAGED, pDevice9,
		NULL, NULL, NULL, NULL, &m_pMesh);*/
	//m_pTex = DX9::GetInstance()->GetTexture("shaderResource/Earth.jpg");

	D3DXCreateSphere(pDevice9, 1.5f, 20, 20, &m_pMesh, NULL);
	
	m_boundingSphere.radius = 1.5f;
	m_boundingSphere.center = gameObject->transform->GetPosition();

	m_material = DXUtil::WHITE_MTRL;
}

void ComMeshSphere::Update()
{
}

void ComMeshSphere::Render()
{
	pDevice9->SetTransform(D3DTS_WORLD, &gameObject->transform->GetWorldMatrix());
	pDevice9->SetMaterial(&m_material);
	m_pMesh->DrawSubset(0);
}
