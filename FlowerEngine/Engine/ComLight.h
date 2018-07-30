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
	static int m_iLightIndex;	// 전체 라이트의 인덱스
	int m_iIndex;				// 이 라이트의 인덱스
	D3DLIGHT9 m_Light;
	Color m_Color;			// 이 라이트의 색상

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

