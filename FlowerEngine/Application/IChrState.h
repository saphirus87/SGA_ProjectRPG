#pragma once
#include "ChrState.h"

class ChrState;

class IChrState
{
public:
	IChrState(){}
	virtual ~IChrState(){}

	virtual void Stand(ChrState* pChrState) = 0;
	virtual void Walk(ChrState* pChrState) = 0;
};
