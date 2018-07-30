/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
�� �ڵ�� ������� ��ī���� ���־� ������ �����ð��� ��� �ڵ带 ���踦 �����Ͽ� ������ �ڵ� �Դϴ�.

���渻 :
ť�긦 ������ ���ִ� �ڵ��Դϴ�.
*/
#pragma once
#include "stdafx.h"

class ComRenderCubePN : public Component
{
public:
	ComRenderCubePN(CString szName);
	~ComRenderCubePN();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void RenderUseShader();
	void RenderRambert();
	bool IsRenderShader;

private:
	Device9			pDevice9;
	VertexBuffer	m_pVB; // ���۸� ����ؼ� �׸���
	IndexBuffer		m_pIB;

	vector<VERTEX_PN> m_verticesPN;
	vector<WORD> m_indices;

	int m_iVertexCnt;
	int m_iPrimitiveCnt;
};

