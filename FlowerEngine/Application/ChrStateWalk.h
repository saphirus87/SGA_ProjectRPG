#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComCharacter;

class ChrStateWalk : public IChrState
{
private:
	ComCharacter * m_pControl;

public:
	ChrStateWalk(ComCharacter* pControl);
	~ChrStateWalk();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
	virtual void Skill1(int iIndex) override;
	virtual void Skill2(int iIndex) override;
	virtual void Death(int iIndex) override;
};

