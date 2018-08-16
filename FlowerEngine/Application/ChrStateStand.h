#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComChrControl;

class ChrStateStand : public IChrState
{
private:
	ComRenderSkinnedMesh * m_pAnimation;

public:
	ChrStateStand(ComRenderSkinnedMesh* pAnimation);
	~ChrStateStand();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(ComChrControl * pChrState) override;
	virtual void Walk(ComChrControl * pChrState) override;
	virtual void Attack1(ComChrControl * pChrState) override;
	virtual void Attack2(ComChrControl * pChrState) override;
	virtual void Attack3(ComChrControl * pChrState) override;
};

