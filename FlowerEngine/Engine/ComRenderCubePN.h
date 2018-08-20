/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
이 코드는 서울게임 아카데미 김주안 선생님 수업시간에 배운 코드를 설계를 수정하여 적용한 코드 입니다.

남길말 :
큐브를 렌더링 해주는 코드입니다.
*/
#pragma once
#include "stdafx.h"

class ComRenderCubePN : public Component
{
public:
	ComRenderCubePN(CString szName);
	~ComRenderCubePN();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void RenderRambert();
	bool IsRenderShader;
	// 렌더링 할지 여부
	bool IsRender;
	
	void SetLocalVertexScale(Vector3& vScale);

	// 삼각형 충돌검사를 위해 WorldMatrix 적용된 모든 삼각형 정점들을 반환합니다.
	vector<Vector3>& GetVector();

	// 일단 public
	ComCollider* pCollider;

private:
	Device9			pDevice9;
	Shader			m_pEffect;

	VertexBuffer	m_pVB; // 버퍼를 사용해서 그리기
	IndexBuffer		m_pIB;
	VertexDecl		m_pVertexDecl;

	vector<VERTEX_PN> m_verticesPN;
	vector<Vector3> m_vecP;
	vector<WORD> m_indices;

	Material		m_material;
	
	int m_iVertexCnt;
	int m_iPrimitiveCnt;
};

