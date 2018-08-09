#pragma once
#include "stdafx.h"

class IChrState;

class ChrState
{
private:
	IChrState * m_pChrState;

public:
	ChrState();
	~ChrState();

	void SetState(IChrState* pChrState);
	void Stand();
	void Walk();
};

