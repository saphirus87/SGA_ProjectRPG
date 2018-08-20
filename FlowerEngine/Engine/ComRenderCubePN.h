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

	void RenderRambert();
	bool IsRenderShader;
	// ������ ���� ����
	bool IsRender;
	
	void SetLocalVertexScale(Vector3& vScale);

	// �ﰢ�� �浹�˻縦 ���� WorldMatrix ����� ��� �ﰢ�� �������� ��ȯ�մϴ�.
	vector<Vector3>& GetVector();

	// �ϴ� public
	ComCollider* pCollider;

private:
	Device9			pDevice9;
	Shader			m_pEffect;

	VertexBuffer	m_pVB; // ���۸� ����ؼ� �׸���
	IndexBuffer		m_pIB;
	VertexDecl		m_pVertexDecl;

	vector<VERTEX_PN> m_verticesPN;
	vector<Vector3> m_vecP;
	vector<WORD> m_indices;

	Material		m_material;
	
	int m_iVertexCnt;
	int m_iPrimitiveCnt;
};

