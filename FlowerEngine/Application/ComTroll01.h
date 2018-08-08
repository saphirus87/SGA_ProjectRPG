#pragma once
#include "stdafx.h"

class ComTroll01 : public Component
{
public:
	ComTroll01(CString szName);
	~ComTroll01();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ComRenderSkinnedMesh * m_pAnimation;
};

