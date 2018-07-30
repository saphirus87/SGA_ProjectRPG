#include "stdafx.h"
#include "ComRenderCubePN.h"

ComRenderCubePN::ComRenderCubePN(CString szName) :
	Component(szName),
	IsRenderShader(true)
{
	pDevice9 = GetD3D9Device();
	// ��� ���ʹ� �߽������� �� ����
	for (size_t i = 0; i < g_vecCubeVertex.size(); ++i)
	{
		// �ش� �������� �߽����� ���� �߽ɿ��� �ش� �������� ���ϴ� ���Ͱ� ������
		// ��, ���� �ݴ���� ����
		Vector3 vecNormal = g_vecCubeVertex[i] - Vector3(0.0f, 0.0f, 0.0f);
		D3DXVec3Normalize(&vecNormal, &vecNormal);
		m_verticesPN.push_back(VERTEX_PN(g_vecCubeVertex[i], vecNormal));
	}

	m_indices = g_vecCubeIndex;
		
	// size��ŭ �޸𸮸� ����̽��� ���� (�׷��� �޸�)
	pDevice9->CreateVertexBuffer(m_verticesPN.size() * sizeof(VERTEX_PN), NULL, VERTEX_PN::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	VERTEX_PN* pVertex; // �� ������ ������
	m_pVB->Lock(0, 0, (LPVOID*)&pVertex, 0); // 0 : ������ ���� ũ�� ��ü�� �޾ƿ��ڴ� 
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

// �ֺ� �� ������ �ȹ���;;
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

	//// �ϳ��� ������ ���� �� �ִ� ���� ������ �ִ�ġ
	//D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };

	//// ���� ���� ���� ������ ��
	//D3DXDeclaratorFromFVF(VERTEX_PN::FVF, vertexDecl);

	//// ���� ���� ����
	//LPDIRECT3DVERTEXDECLARATION9 pDecl = NULL;
	//pDevice9->CreateVertexDeclaration(vertexDecl, &pDecl);
	//pDevice9->SetVertexDeclaration(pDecl);

	//// �׸��� ������ �� ���� pass�� �߰����־�� ��
	//UINT numPasses = 0;
	//pEffect->Begin(&numPasses, NULL);
	////pEffect->SetTexture("DiffuseMap_Tex", m_pTex);

	//for (int i = 0; i < numPasses; ++i)
	//{
	//	pEffect->BeginPass(i);
	//	// ������ �ٲ���ٴ� ���� �˸�
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

	// ����-���� ��ȯ����� ����ġ���
	Matrix4x4 m;
	D3DXMatrixInverse(&m, NULL, &gameObject->transform->GetWorldMatrix());
	D3DXMatrixTranspose(&m, &m);
	pEffect->SetMatrix("mWIT", &m);

	// �ϳ��� ������ ���� �� �ִ� ���� ������ �ִ�ġ
	D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };

	// ���� ���� ���� ������ ��
	D3DXDeclaratorFromFVF(VERTEX_PN::FVF, vertexDecl);

	// ���� ���� ����
	LPDIRECT3DVERTEXDECLARATION9 pDecl = NULL;
	pDevice9->CreateVertexDeclaration(vertexDecl, &pDecl);
	pDevice9->SetVertexDeclaration(pDecl);

	// �׸��� ������ �� ���� pass�� �߰����־�� ��
	UINT numPasses = 0;
	pEffect->Begin(&numPasses, NULL);
	//pEffect->SetTexture("DiffuseMap_Tex", m_pTex);

	D3DXVECTOR4 v;

	for (int i = 0; i < numPasses; ++i)
	{
		pEffect->BeginPass(i);
		pDevice9->SetFVF(VERTEX_PN::FVF);
		// ������ �ٲ���ٴ� ���� �˸�
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
