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
};

