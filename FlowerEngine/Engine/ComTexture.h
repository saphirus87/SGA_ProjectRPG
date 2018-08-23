#pragma once
#include "stdafx.h"

class ComTexture : public Component
{
public:
	ComTexture(CString szName);
	~ComTexture();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	Texture			pTexture;

private:
	// 다이렉트X 디바이스
	Device9			m_pDevice9;

	// 정점을 담을 버퍼
	VertexBuffer	m_pVB;
	// 정점 인덱스를 담을 버퍼
	IndexBuffer		m_pIB;

	// 정점과 UV정보를 담은 벡터(배열)
	vector<VERTEX_PT> m_verticesPT;
	// 정점 인덱스를 담은 벡터(배열)
	vector<WORD> m_indices;

	// 정점의 갯수
	int m_iVertexCnt;
	// 삼각형 갯수
	int m_iPrimitiveCnt;
};

