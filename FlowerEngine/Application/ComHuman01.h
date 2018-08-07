/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

설명 :
캐릭터중 인간입니다.
*/
#pragma once
#include "stdafx.h"

class ComObjMap;

class ComHuman01 : public Component
{
public:
	ComHuman01(CString szName);
	~ComHuman01();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ComRenderSkinnedMesh * m_pAnimation;
	ComObjMap* m_pMap;

	// 어깨 장착 관련
	ComRenderXMesh* m_pSholderRight;
	ComRenderXMesh* m_pSholderLeft;
	Matrix4x4 m_matSholderRight;
	Matrix4x4 m_matSholderLeft;
};

