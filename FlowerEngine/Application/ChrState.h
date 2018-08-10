#pragma once
#include "stdafx.h"

class IChrState;

class ChrState
{
private:
	IChrState * m_pChrState;
	ComRenderSkinnedMesh * m_pAnimation;

public:
	ChrState(ComRenderSkinnedMesh* pAnimation);
	~ChrState();

	void SetState(IChrState* pChrState);
	void Stand();
	void Walk();
};

