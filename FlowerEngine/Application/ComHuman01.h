/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

���� :
ĳ������ �ΰ��Դϴ�.
*/
#pragma once
#include "stdafx.h"

class ComObjMap;

class ComHuman01 : public Component
{
public:
	ComHuman01(CString szName);
	~ComHuman01();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ComRenderSkinnedMesh * m_pAnimation;
	ComObjMap* m_pMap;

	// ��� ���� ����
	ComRenderXMesh* m_pSholderRight;
	ComRenderXMesh* m_pSholderLeft;
	Matrix4x4 m_matSholderRight;
	Matrix4x4 m_matSholderLeft;
};

