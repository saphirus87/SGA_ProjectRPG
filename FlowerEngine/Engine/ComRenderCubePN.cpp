#include "stdafx.h"
#include "ComRenderCubePN.h"

ComRenderCubePN::ComRenderCubePN(CString szName) :
	Component(szName),
	m_pVB(NULL),
	m_pIB(NULL),
	IsRenderShader(true),
	IsRender(true),
	pCollider(NULL)
{
	pDevice9 = GetD3D9Device();
	
	SetLocalVertexScale(Vector3(1, 1, 1));
	
	pDevice9->CreateIndexBuffer(m_indices.size() * sizeof(WORD), NULL, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL);
	WORD* pIndex;
	m_pIB->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, &m_indices[0], m_indices.size() * sizeof(WORD));
	m_pIB->Unlock();
	m_iPrimitiveCnt = m_indices.size() / 3.0f;
	m_indices.clear();
}

ComRenderCubePN::~ComRenderCubePN()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVertexDecl);
}

void ComRenderCubePN::Update()
{
	//if (Input::GetInstance()->KeyDown('R'))
	//	IsRenderShader = !IsRenderShader;
	if (Input::GetInstance()->KeyDown(VK_F2))
		IsRender = !IsRender;
}

void ComRenderCubePN::Render()
{
	if (IsRender == false)
		return;

	if (IsRenderShader)
		RenderRambert();
	else
	{
		pDevice9->SetTransform(D3DTS_WORLD, &gameObject->transform->GetWorldMatrix());
		pDevice9->SetMaterial(&m_material);
		pDevice9->SetFVF(VERTEX_PN::FVF);
		pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PN));
		pDevice9->SetIndices(m_pIB);
		pDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCnt, 0, m_iPrimitiveCnt);
	}
}

void ComRenderCubePN::RenderRambert()
{
	Vector3 offsetPos = pCollider->offsetPos;
	Matrix4x4 matLocal;
	D3DXMatrixIdentity(&matLocal);
	D3DXMatrixTranslation(&matLocal, offsetPos.x, offsetPos.y, offsetPos.z);

	Matrix4x4 matWorld = matLocal * gameObject->transform->GetWorldMatrix();

	m_pEffect->SetMatrix("gWorldMatrix", &matWorld);
	m_pEffect->SetMatrix("gViewMatrix", &Camera::GetInstance()->GetViewMatrix());
	m_pEffect->SetMatrix("gProjMatrix", &Camera::GetInstance()->GetProjMatrix());

	m_pEffect->SetVector("vLightDir", &D3DXVECTOR4(-0.577f, -0.577f, -0.577f, 0));

	// ����-���� ��ȯ����� ����ġ���
	Matrix4x4 m;
	D3DXMatrixInverse(&m, NULL, &gameObject->transform->GetWorldMatrix());
	D3DXMatrixTranspose(&m, &m);
	m_pEffect->SetMatrix("mWIT", &m);

	// �׸��� ������ �� ���� pass�� �߰����־�� ��
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	pDevice9->SetVertexDeclaration(m_pVertexDecl);
	//pEffect->SetTexture("DiffuseMap_Tex", m_pTex);

	D3DXVECTOR4 v;

	for (int i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		pDevice9->SetFVF(VERTEX_PN::FVF);

		v.x = m_material.Diffuse.r;
		v.y = m_material.Diffuse.g;
		v.z = m_material.Diffuse.b;
		v.w = m_material.Diffuse.a;

		m_pEffect->SetVector("k_d", &v);
		m_pEffect->SetVector("k_a", &v);

		// ������ �ٲ���ٴ� ���� �˸�
		m_pEffect->CommitChanges();
		
		pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PN));
		pDevice9->SetIndices(m_pIB);
		pDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCnt, 0, m_iPrimitiveCnt);

		m_pEffect->EndPass();
	}

	m_pEffect->End();
}

void ComRenderCubePN::SetLocalVertexScale(Vector3 & vScale)
{
	SAFE_RELEASE(m_pVB);
	m_verticesPN.clear();
	
	// ��� ���ʹ� �߽������� �� ����
	for (size_t i = 0; i < g_vecCubeVertex.size(); ++i)
	{
		// �ش� �������� �߽����� ���� �߽ɿ��� �ش� �������� ���ϴ� ���Ͱ� ������
		// ��, ���� �ݴ���� ����
		Vector3 vecNormal = g_vecCubeVertex[i] - Vector3(0.0f, 0.0f, 0.0f);
		D3DXVec3Normalize(&vecNormal, &vecNormal);
		Vector3 v = g_vecCubeVertex[i];
		v.x *= vScale.x;
		v.y *= vScale.y;
		v.z *= vScale.z;
		m_verticesPN.push_back(VERTEX_PN(v, vecNormal));
	}

	m_indices = g_vecCubeIndex;

	// size��ŭ �޸𸮸� ����̽��� ���� (�׷��� �޸�)
	pDevice9->CreateVertexBuffer(m_verticesPN.size() * sizeof(VERTEX_PN), NULL, VERTEX_PN::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	VERTEX_PN* pVertex; // �� ������ ������
	m_pVB->Lock(0, 0, (LPVOID*)&pVertex, 0); // 0 : ������ ���� ũ�� ��ü�� �޾ƿ��ڴ� 
	memcpy(pVertex, &m_verticesPN[0], m_verticesPN.size() * sizeof(VERTEX_PN));
	m_pVB->Unlock();
	m_iVertexCnt = m_verticesPN.size();
}

vector<Vector3>& ComRenderCubePN::GetVector()
{
	m_vecP.clear();
	for (size_t i = 0; i < g_vecCubeIndex.size(); ++i)
		m_vecP.push_back(m_verticesPN[g_vecCubeIndex[i]].p);

	Vector3& pos = gameObject->transform->GetPosition();
	Matrix4x4 matT;
	D3DXMatrixIdentity(&matT);
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);

	for (size_t i = 0; i < m_vecP.size(); ++i)
		D3DXVec3TransformCoord(&m_vecP[i], &m_vecP[i], &matT);

	return m_vecP;
}

void ComRenderCubePN::Awake()
{
	// �ϳ��� ������ ���� �� �ִ� ���� ������ �ִ�ġ
	D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };

	// ���� ���� ���� ������ ��
	D3DXDeclaratorFromFVF(VERTEX_PN::FVF, vertexDecl);

	// ���� ���� ����
	pDevice9->CreateVertexDeclaration(vertexDecl, &m_pVertexDecl);
	pDevice9->SetVertexDeclaration(m_pVertexDecl);

	m_material = DXUtil::WHITE_MTRL;
	m_pEffect = Shaders::GetInstance()->GetShader("./Resources/shader/ShaderRambert.fx");
}
