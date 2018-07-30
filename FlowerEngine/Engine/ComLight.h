#pragma once
#include "stdafx.h"

class ComLight : public Component
{
public:
	ComLight(CString szName);
	~ComLight();

	void InitDirectional(float fAmbient = 0.1f, float fDiffuse = 0.2f);
	void InitPoint(float fPosX, float fPosY, float fPosZ, Color* c);

	void UpdateLight(float fAmbient, float fDiffuse);

private:
	void InitLight();

private:
	Device9 pDevice9;
	static int m_iLightIndex;	// ��ü ����Ʈ�� �ε���
	int m_iIndex;				// �� ����Ʈ�� �ε���
	D3DLIGHT9 m_Light;
	Color m_Color;			// �� ����Ʈ�� ����

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

