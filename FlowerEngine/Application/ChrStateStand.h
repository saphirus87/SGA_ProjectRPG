#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ChrState;

class ChrStateStand : public IChrState
{
private:
	ChrState * m_pChrState;

public:
	ChrStateStand();
	~ChrStateStand();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(ChrState * pChrState) override;
	virtual void Walk(ChrState * pChrState) override;
};

