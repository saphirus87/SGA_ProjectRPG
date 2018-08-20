#pragma once
#include "stdafx.h"

// ĳ������ �������Դϴ�.
class ComCharacter : public Component
{
public:
	ComCharacter(CString szName);
	virtual ~ComCharacter();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

protected:
	void Init();

protected:
	ComRenderSkinnedMesh * m_pAnimation;
};

