#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComChrControl;

class ChrStateStand : public IChrState
{
private:
	ComChrControl * m_pControl;

public:
	ChrStateStand(ComChrControl* pControl);
	~ChrStateStand();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
};

