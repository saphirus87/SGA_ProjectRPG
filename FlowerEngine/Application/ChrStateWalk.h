#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComChrControl;

class ChrStateWalk : public IChrState
{
private:
	//ComChrControl * m_pChrState;
	ComRenderSkinnedMesh * m_pAnimation;

public:
	ChrStateWalk(ComRenderSkinnedMesh* pAnimation);
	~ChrStateWalk();

	// IChrState을(를) 통해 상속됨
	virtual void Stand(ComChrControl * pChrState) override;
	virtual void Walk(ComChrControl * pChrState) override;
	virtual void Attack(ComChrControl * pChrState) override;
};

