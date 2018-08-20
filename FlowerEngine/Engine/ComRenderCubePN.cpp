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

	// 로컬-월드 변환행렬의 역전치행렬
	Matrix4x4 m;
	D3DXMatrixInverse(&m, NULL, &gameObject->transform->GetWorldMatrix());
	D3DXMatrixTranspose(&m, &m);
	m_pEffect->SetMatrix("mWIT", &m);

	// 그림자 같은거 할 때도 pass를 추가해주어야 함
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

		// 변수가 바뀌었다는 것을 알림
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
	
	// 노멀 벡터는 중심점에서 그 벡터
	for (size_t i = 0; i < g_vecCubeVertex.size(); ++i)
	{
		// 해당 정점에서 중심점을 빼면 중심에서 해당 정점으로 향하는 벡터가 구해짐
		// 즉, 빼는 반대방향 벡터
		Vector3 vecNormal = g_vecCubeVertex[i] - Vector3(0.0f, 0.0f, 0.0f);
		D3DXVec3Normalize(&vecNormal, &vecNormal);
		Vector3 v = g_vecCubeVertex[i];
		v.x *= vScale.x;
		v.y *= vScale.y;
		v.z *= vScale.z;
		m_verticesPN.push_back(VERTEX_PN(v, vecNormal));
	}

	m_indices = g_vecCubeIndex;

	// size만큼 메모리를 디바이스에 잡음 (그래픽 메모리)
	pDevice9->CreateVertexBuffer(m_verticesPN.size() * sizeof(VERTEX_PN), NULL, VERTEX_PN::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	VERTEX_PN* pVertex; // 이 버퍼의 포인터
	m_pVB->Lock(0, 0, (LPVOID*)&pVertex, 0); // 0 : 생성한 버퍼 크기 전체를 받아오겠다 
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
	// 하나의 정점이 가질 수 있는 정보 갯수의 최대치
	D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };

	// 정점 선언 정보 가지고 옴
	D3DXDeclaratorFromFVF(VERTEX_PN::FVF, vertexDecl);

	// 정점 선언 정보
	pDevice9->CreateVertexDeclaration(vertexDecl, &m_pVertexDecl);
	pDevice9->SetVertexDeclaration(m_pVertexDecl);

	m_material = DXUtil::WHITE_MTRL;
	m_pEffect = Shaders::GetInstance()->GetShader("./Resources/shader/ShaderRambert.fx");
}
