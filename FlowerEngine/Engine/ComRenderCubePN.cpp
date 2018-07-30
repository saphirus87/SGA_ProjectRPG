#include "stdafx.h"
#include "ComRenderCubePN.h"

ComRenderCubePN::ComRenderCubePN(CString szName) :
	Component(szName),
	IsRenderShader(true)
{
	pDevice9 = GetD3D9Device();
	// 노멀 벡터는 중심점에서 그 벡터
	for (size_t i = 0; i < g_vecCubeVertex.size(); ++i)
	{
		// 해당 정점에서 중심점을 빼면 중심에서 해당 정점으로 향하는 벡터가 구해짐
		// 즉, 빼는 반대방향 벡터
		Vector3 vecNormal = g_vecCubeVertex[i] - Vector3(0.0f, 0.0f, 0.0f);
		D3DXVec3Normalize(&vecNormal, &vecNormal);
		m_verticesPN.push_back(VERTEX_PN(g_vecCubeVertex[i], vecNormal));
	}

	m_indices = g_vecCubeIndex;
		
	// size만큼 메모리를 디바이스에 잡음 (그래픽 메모리)
	pDevice9->CreateVertexBuffer(m_verticesPN.size() * sizeof(VERTEX_PN), NULL, VERTEX_PN::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	VERTEX_PN* pVertex; // 이 버퍼의 포인터
	m_pVB->Lock(0, 0, (LPVOID*)&pVertex, 0); // 0 : 생성한 버퍼 크기 전체를 받아오겠다 
	memcpy(pVertex, &m_verticesPN[0], m_verticesPN.size() * sizeof(VERTEX_PN));
	m_pVB->Unlock();
	m_iVertexCnt = m_verticesPN.size();
	m_verticesPN.clear();

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
}

void ComRenderCubePN::Update()
{
	if (Input::GetInstance()->KeyDown('R'))
		IsRenderShader = !IsRenderShader;
}

void ComRenderCubePN::Render()
{
	if (IsRenderShader)
		RenderRambert();
		//RenderUseShader();
	else
	{
		pDevice9->SetTransform(D3DTS_WORLD, &gameObject->transform->GetWorldMatrix());
		pDevice9->SetMaterial(&DXUtil::WHITE_MTRL);
		pDevice9->SetFVF(VERTEX_PN::FVF);
		pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PN));
		pDevice9->SetIndices(m_pIB);
		pDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCnt, 0, m_iPrimitiveCnt);
	}
}

// 주변 빛 영향을 안받음;;
void ComRenderCubePN::RenderUseShader()
{
	//LPD3DXEFFECT pEffect = DX9::GetInstance()->GetShader("./Resources/shader/ShaderLighting.fx");

	//pEffect->SetMatrix("gWorldMatrix", &gameObject->transform->GetWorldMatrix());
	//pEffect->SetMatrix("gViewMatrix", &Camera::GetInstance()->GetViewMatrix());
	//pEffect->SetMatrix("gProjMatrix", &Camera::GetInstance()->GetProjMatrix());

	//D3DXVECTOR4 lightPos = D3DXVECTOR4(-10, 10, -10, 1);
	//Vector3 camPos = Camera::GetInstance()->GetPosition();
	//D3DXVECTOR4 camPos4 = D3DXVECTOR4(camPos.x, camPos.y, camPos.z, 1);

	//pEffect->SetVector("gWorldLightPos", &lightPos);
	//pEffect->SetVector("gWorldCameraPos", &camPos4);

	//// 하나의 정점이 가질 수 있는 정보 갯수의 최대치
	//D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };

	//// 정점 선언 정보 가지고 옴
	//D3DXDeclaratorFromFVF(VERTEX_PN::FVF, vertexDecl);

	//// 정점 선언 정보
	//LPDIRECT3DVERTEXDECLARATION9 pDecl = NULL;
	//pDevice9->CreateVertexDeclaration(vertexDecl, &pDecl);
	//pDevice9->SetVertexDeclaration(pDecl);

	//// 그림자 같은거 할 때도 pass를 추가해주어야 함
	//UINT numPasses = 0;
	//pEffect->Begin(&numPasses, NULL);
	////pEffect->SetTexture("DiffuseMap_Tex", m_pTex);

	//for (int i = 0; i < numPasses; ++i)
	//{
	//	pEffect->BeginPass(i);
	//	// 변수가 바뀌었다는 것을 알림
	//	pEffect->CommitChanges();

	//	pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PN));
	//	pDevice9->SetIndices(m_pIB);
	//	pDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCnt, 0, m_iPrimitiveCnt);

	//	pEffect->EndPass();
	//}

	//pEffect->End();
	//SAFE_RELEASE(pDecl);
}

void ComRenderCubePN::RenderRambert()
{
	LPD3DXEFFECT pEffect = Shaders::GetInstance()->GetShader("./Resources/shader/ShaderRambert.fx");

	pEffect->SetMatrix("gWorldMatrix", &gameObject->transform->GetWorldMatrix());
	pEffect->SetMatrix("gViewMatrix", &Camera::GetInstance()->GetViewMatrix());
	pEffect->SetMatrix("gProjMatrix", &Camera::GetInstance()->GetProjMatrix());

	pEffect->SetVector("vLightDir", &D3DXVECTOR4(-0.577f, -0.577f, -0.577f, 0));

	// 로컬-월드 변환행렬의 역전치행렬
	Matrix4x4 m;
	D3DXMatrixInverse(&m, NULL, &gameObject->transform->GetWorldMatrix());
	D3DXMatrixTranspose(&m, &m);
	pEffect->SetMatrix("mWIT", &m);

	// 하나의 정점이 가질 수 있는 정보 갯수의 최대치
	D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };

	// 정점 선언 정보 가지고 옴
	D3DXDeclaratorFromFVF(VERTEX_PN::FVF, vertexDecl);

	// 정점 선언 정보
	LPDIRECT3DVERTEXDECLARATION9 pDecl = NULL;
	pDevice9->CreateVertexDeclaration(vertexDecl, &pDecl);
	pDevice9->SetVertexDeclaration(pDecl);

	// 그림자 같은거 할 때도 pass를 추가해주어야 함
	UINT numPasses = 0;
	pEffect->Begin(&numPasses, NULL);
	//pEffect->SetTexture("DiffuseMap_Tex", m_pTex);

	D3DXVECTOR4 v;

	for (int i = 0; i < numPasses; ++i)
	{
		pEffect->BeginPass(i);
		pDevice9->SetFVF(VERTEX_PN::FVF);
		// 변수가 바뀌었다는 것을 알림
		pEffect->CommitChanges();

		D3DMATERIAL9 pMtrl = DXUtil::WHITE_MTRL;
		v.x = pMtrl.Diffuse.r;
		v.y = pMtrl.Diffuse.g;
		v.z = pMtrl.Diffuse.b;
		v.w = pMtrl.Diffuse.a;
		pEffect->SetVector("k_d", &v);
		pEffect->SetVector("k_a", &v);

		pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PN));
		pDevice9->SetIndices(m_pIB);
		pDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCnt, 0, m_iPrimitiveCnt);

		pEffect->EndPass();
	}

	pEffect->End();
	SAFE_RELEASE(pDecl);
}

void ComRenderCubePN::Awake()
{
}
