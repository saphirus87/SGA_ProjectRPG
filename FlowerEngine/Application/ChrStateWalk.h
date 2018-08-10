#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ChrState;

class ChrStateWalk : public IChrState
{
private:
	ChrState * m_pChrState;
	ComRenderSkinnedMesh * m_pAnimation;

public:
	ChrStateWalk(ComRenderSkinnedMesh* pAnimation);
	~ChrStateWalk();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(ChrState * pChrState) override;
	virtual void Walk(ChrState * pChrState) override;
};

