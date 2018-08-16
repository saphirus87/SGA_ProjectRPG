#include "stdafx.h"
#include "ComChrControl.h"
#include "ChrStateAttack.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"

ChrStateAttack::ChrStateAttack(ComRenderSkinnedMesh* pAnimation)
{
	m_pAnimation = pAnimation;
	m_pAnimation->PlayAnimation(eAni_Attack_1);
}

ChrStateAttack::~ChrStateAttack()
{
}

void ChrStateAttack::Stand(ComChrControl * pChrState)
{
	OutputDebugString(L"Attack -> Stand 상태 변환\r\n");

	ChrStateStand* pStateStand = new ChrStateStand(m_pAnimation);
	pChrState->SetState(pStateStand);
}

void ChrStateAttack::Walk(ComChrControl * pChrState)
{
	OutputDebugString(L"Attack -> Walk 상태 변환\r\n");

	ChrStateWalk* pStateWalk = new ChrStateWalk(m_pAnimation);
	pChrState->SetState(pStateWalk);
}

void ChrStateAttack::Attack(ComChrControl * pChrState)
{
	// 사용하지 않음
}
