#pragma once
#include "stdafx.h"

class ChrState;

class ComUndead01 : public Component
{
public:
	ComUndead01(CString szName);
	~ComUndead01();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ChrState * m_pChrState;
	ComRenderSkinnedMesh * m_pAnimation;
};

