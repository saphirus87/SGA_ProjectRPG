#include "stdafx.h"
#include "ComChrControl.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"

ChrStateStand::ChrStateStand(ComRenderSkinnedMesh* pAnimation)
{
	m_pAnimation = pAnimation;
	m_pAnimation->PlayAnimation(eAni_Stand);
}

ChrStateStand::~ChrStateStand()
{
}

void ChrStateStand::Stand(ComChrControl * pChrState)
{
	// 사용되지 않음
}

void ChrStateStand::Walk(ComChrControl * pChrState)
{
	OutputDebugString(L"Stand -> Walk 상태 변환\r\n");

	ChrStateWalk* pStateWalk = new ChrStateWalk(m_pAnimation);
	pChrState->SetState(pStateWalk);
}

void ChrStateStand::Attack1(ComChrControl * pChrState)
{
	OutputDebugString(L"Stand -> Attack1 상태 변환\r\n");

	ChrStateAttack1* pStateAttack = new ChrStateAttack1(m_pAnimation);
	pChrState->SetState(pStateAttack);
}

void ChrStateStand::Attack2(ComChrControl * pChrState)
{
}

void ChrStateStand::Attack3(ComChrControl * pChrState)
{
}
