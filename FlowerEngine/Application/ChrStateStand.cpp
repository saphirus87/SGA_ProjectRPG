#include "stdafx.h"
#include "ChrStateStand.h"
#include "ChrState.h"
#include "ChrStateWalk.h"

ChrStateStand::ChrStateStand(ComRenderSkinnedMesh* pAnimation)
{
	m_pAnimation = pAnimation;
}

ChrStateStand::~ChrStateStand()
{
}

void ChrStateStand::Stand(ChrState * pChrState)
{
	//Stand ����
	m_pAnimation->PlayAnimation(eAni_Stand);
}

void ChrStateStand::Walk(ChrState * pChrState)
{
	//Stand -> Walk ���º�ȯ
	m_pChrState->SetState(new ChrStateWalk(m_pAnimation));
}
