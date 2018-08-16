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

void ChrStateStand::Attack(ComChrControl * pChrState)
{
	OutputDebugString(L"Stand -> Attack 상태 변환\r\n");

	ChrStateAttack* pStateAttack = new ChrStateAttack(m_pAnimation);
	pChrState->SetState(pStateAttack);
}
