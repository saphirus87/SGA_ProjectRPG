#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComChrControl;

class ChrStateAttack1 : public IChrState
{
private:
	ComChrControl * m_pControl;

public:
	ChrStateAttack1(ComChrControl* pControl);
	~ChrStateAttack1();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
};