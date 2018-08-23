#pragma once
#include "stdafx.h"

class ComTexture : public Component
{
public:
	ComTexture(CString szName);
	~ComTexture();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void Set(Texture pTex, bool isBillboard = false);

	Texture pTexture;

	// ī�޶� �ٶ�
	bool IsBillboard;

private:
	// ���̷�ƮX ����̽�
	Device9			m_pDevice9;

	// ������ ���� ����
	VertexBuffer	m_pVB;
	// ���� �ε����� ���� ����
	IndexBuffer		m_pIB;

	// ������ UV������ ���� ����(�迭)
	vector<VERTEX_PT> m_verticesPT;
	// ���� �ε����� ���� ����(�迭)
	vector<WORD> m_indices;

	// ������ ����
	int m_iVertexCnt;
	// �ﰢ�� ����
	int m_iPrimitiveCnt;

	// ī�޶� �ٶ󺸰� �ϱ� ����
	Matrix4x4 m_matBillboard;
};

