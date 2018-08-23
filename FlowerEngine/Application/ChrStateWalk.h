#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComChrControl;

class ChrStateWalk : public IChrState
{
private:
	ComChrControl * m_pControl;

public:
	ChrStateWalk(ComChrControl* pControl);
	~ChrStateWalk();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
	virtual void Death(int iIndex) override;
};

