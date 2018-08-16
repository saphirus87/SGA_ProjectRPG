#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComChrControl;

class ChrStateWalk : public IChrState
{
private:
	ComRenderSkinnedMesh * m_pAnimation;

public:
	ChrStateWalk(ComRenderSkinnedMesh* pAnimation);
	~ChrStateWalk();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(ComChrControl * pChrState) override;
	virtual void Walk(ComChrControl * pChrState) override;
	virtual void Attack1(ComChrControl * pChrState) override;
	virtual void Attack2(ComChrControl * pChrState) override;
	virtual void Attack3(ComChrControl * pChrState) override;
};

