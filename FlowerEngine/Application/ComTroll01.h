#pragma once
#include "stdafx.h"

class ComObjMap;

class ComTroll01 : public Component
{
public:
	ComTroll01(CString szName);
	~ComTroll01();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ComRenderSkinnedMesh * m_pAnimation;
	ComObjMap* m_pMap;
};

