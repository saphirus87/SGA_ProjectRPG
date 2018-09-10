#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComCharacter;

class ChrStateStand : public IChrState
{
private:
	ComCharacter * m_pControl;

public:
	ChrStateStand(ComCharacter* pControl);
	~ChrStateStand();

	// IChrState을(를) 통해 상속됨
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
	virtual void Skill1(int iIndex) override;
	virtual void Skill2(int iIndex) override;
	virtual void Death(int iIndex) override;
};

class ChrStateDeath : public IChrState
{
private:
	ComCharacter * m_pControl;

public:
	ChrStateDeath(ComCharacter* pControl);
	~ChrStateDeath();

	// IChrState을(를) 통해 상속됨
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
	virtual void Skill1(int iIndex) override;
	virtual void Skill2(int iIndex) override;
	virtual void Death(int iIndex) override;
};
