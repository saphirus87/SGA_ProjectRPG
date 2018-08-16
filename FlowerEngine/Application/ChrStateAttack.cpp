#include "stdafx.h"
#include "ComChrControl.h"
#include "ChrStateAttack.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"

ChrStateAttack1::ChrStateAttack1(ComRenderSkinnedMesh* pAnimation)
{
	m_pAnimation = pAnimation;
	m_pAnimation->PlayAnimation(eAni_Attack_1);
}

ChrStateAttack1::~ChrStateAttack1()
{
}

void ChrStateAttack1::Stand(ComChrControl * pChrState)
{
	OutputDebugString(L"Attack1 -> Stand 상태 변환\r\n");

	ChrStateStand* pStateStand = new ChrStateStand(m_pAnimation);
	pChrState->SetState(pStateStand);
}

void ChrStateAttack1::Walk(ComChrControl * pChrState)
{
	OutputDebugString(L"Attack1 -> Walk 상태 변환\r\n");

	ChrStateWalk* pStateWalk = new ChrStateWalk(m_pAnimation);
	pChrState->SetState(pStateWalk);
}

void ChrStateAttack1::Attack1(ComChrControl * pChrState)
{
	// 사용하지 않음
}

void ChrStateAttack1::Attack2(ComChrControl * pChrState)
{
}

void ChrStateAttack1::Attack3(ComChrControl * pChrState)
{
}

ChrStateAttack2::ChrStateAttack2(ComRenderSkinnedMesh * pAnimation)
{
}

ChrStateAttack2::~ChrStateAttack2()
{
}

void ChrStateAttack2::Stand(ComChrControl * pChrState)
{
}

void ChrStateAttack2::Walk(ComChrControl * pChrState)
{
}

void ChrStateAttack2::Attack1(ComChrControl * pChrState)
{
}

void ChrStateAttack2::Attack2(ComChrControl * pChrState)
{
}

void ChrStateAttack2::Attack3(ComChrControl * pChrState)
{
}

ChrStateAttack3::ChrStateAttack3(ComRenderSkinnedMesh * pAnimation)
{
}

ChrStateAttack3::~ChrStateAttack3()
{
}

void ChrStateAttack3::Stand(ComChrControl * pChrState)
{
}

void ChrStateAttack3::Walk(ComChrControl * pChrState)
{
}

void ChrStateAttack3::Attack1(ComChrControl * pChrState)
{
}

void ChrStateAttack3::Attack2(ComChrControl * pChrState)
{
}

void ChrStateAttack3::Attack3(ComChrControl * pChrState)
{
}
