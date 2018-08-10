#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ChrState;

class ChrStateStand : public IChrState
{
private:
	ChrState * m_pChrState;
	ComRenderSkinnedMesh * m_pAnimation;

public:
	ChrStateStand(ComRenderSkinnedMesh* pAnimation);
	~ChrStateStand();

	// IChrState을(를) 통해 상속됨
	virtual void Stand(ChrState * pChrState) override;
	virtual void Walk(ChrState * pChrState) override;
};

