#pragma once

class ComChrControl;

class IChrState
{
public:
	IChrState(){}
	virtual ~IChrState(){}

	virtual void Stand(ComChrControl* pChrState) = 0;
	virtual void Walk(ComChrControl* pChrState) = 0;
	virtual void Attack(ComChrControl* pChrState) = 0;
};
