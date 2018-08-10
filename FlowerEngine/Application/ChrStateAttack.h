#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ChrState;

class ChrStateAttack : public IChrState
{
private:
	ChrState * m_pChrState;
	ComRenderSkinnedMesh * m_pAnimation;

public:
	ChrStateAttack(ComRenderSkinnedMesh* pAnimation);
	~ChrStateAttack();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(ChrState * pChrState) override;
	virtual void Walk(ChrState * pChrState) override;
	virtual void Attack(ChrState * pChrState) override;
};

