#pragma once
#include "stdafx.h"

class ChrState;

class ComUndead01 : public Component
{
public:
	ComUndead01(CString szName);
	~ComUndead01();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ComRenderSkinnedMesh * m_pAnimation;
};

