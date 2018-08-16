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
	virtual void Attack(ComChrControl * pChrState) override;
};

