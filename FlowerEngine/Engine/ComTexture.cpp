#include "stdafx.h"
#include "ComTexture.h"

ComTexture::ComTexture(CString szName)
	:Component(szName), 
	m_pVB(NULL), m_pIB(NULL), pTexture(NULL), 
	m_iVertexCnt(0), m_iPrimitiveCnt(0)
{
	m_pDevice9 = GetD3D9Device();
}

ComTexture::~ComTexture()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

void ComTexture::Awake()
{
	pTexture = Assets::GetTexture(L"Resources/ui/panel-info.png");

	//정점 4개를 만듬
	VERTEX_PT v0;
	v0.p = Vector3(-0.5f, -0.5f, 0);
	v0.t = Vector2(0, 1);
	m_verticesPT.push_back(v0);


	VERTEX_PT v1;
	v1.p = Vector3(-0.5f, 0.5f, 0);
	v1.t = Vector2(0, 0);
	m_verticesPT.push_back(v1);


	VERTEX_PT v2;
	v2.p = Vector3(0.5f, 0.5f, 0);
	v2.t = Vector2(1, 0);
	m_verticesPT.push_back(v2);

	VERTEX_PT v3;
	v3.p = Vector3(0.5f, -0.5f, 0);
	v3.t = Vector2(1, 1);
	m_verticesPT.push_back(v3);

	// 정점 갯수 선언
	m_iVertexCnt = m_verticesPT.size();

	// 정점 버퍼 생성
	size_t bufferSize = m_verticesPT.size() * sizeof(VERTEX_PT);
	m_pDevice9->CreateVertexBuffer(bufferSize, NULL, VERTEX_PT::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	VERTEX_PT* pVertex;	// 버퍼의 포인터
	m_pVB->Lock(0, 0, (LPVOID*)&pVertex, 0);
	memcpy(pVertex, &m_verticesPT[0], bufferSize);
	m_pVB->Unlock();

	//인덱스 선언
	m_indices.push_back(0);
	m_indices.push_back(1);
	m_indices.push_back(2);

	m_indices.push_back(0);
	m_indices.push_back(2);
	m_indices.push_back(3);

	m_iPrimitiveCnt = m_indices.size() / 3.0f;
	bufferSize = m_indices.size() * sizeof(WORD);
	//인덱스 버퍼 생성
	m_pDevice9->CreateIndexBuffer(bufferSize, NULL, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL);
	WORD* pIndex;	//인덱스 버퍼의 포인터
	m_pIB->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, &m_indices[0], bufferSize);
	m_pIB->Unlock();

	m_indices.clear();
}

void ComTexture::Update()
{
}

void ComTexture::Render()
{
	m_pDevice9->SetRenderState(D3DRS_LIGHTING, false);
	m_pDevice9->SetTransform(D3DTS_WORLD, &gameObject->transform->GetWorldMatrix());
	m_pDevice9->SetFVF(VERTEX_PT::FVF);
	m_pDevice9->SetTexture(0, pTexture);
	m_pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PT));
	m_pDevice9->SetIndices(m_pIB);
	m_pDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCnt, 0, m_iPrimitiveCnt);
	m_pDevice9->SetRenderState(D3DRS_LIGHTING, true);
}
