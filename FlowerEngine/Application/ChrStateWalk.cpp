#include "stdafx.h"
#include "ChrStateWalk.h"
#include "ChrState.h"
#include "ChrStateStand.h"

ChrStateWalk::ChrStateWalk(ComRenderSkinnedMesh* pAnimation)
{
	m_pAnimation = pAnimation;
}

ChrStateWalk::~ChrStateWalk()
{
	//SAFE_DELETE(m_pChrState);
}

void ChrStateWalk::Stand(ChrState * pChrState)
{
	// Walk -> Stand ���� ��ȯ
	m_pChrState->SetState(new ChrStateStand(m_pAnimation));
}

void ChrStateWalk::Walk(ChrState * pChrState)
{
	// Walk ����
	m_pAnimation->PlayAnimation(eAni_Walk);
}
