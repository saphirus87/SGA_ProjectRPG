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
	// ������ ����
}

void ChrStateStand::Walk(ComChrControl * pChrState)
{
	OutputDebugString(L"Stand -> Walk ���� ��ȯ\r\n");

	ChrStateWalk* pStateWalk = new ChrStateWalk(m_pAnimation);
	pChrState->SetState(pStateWalk);
}

void ChrStateStand::Attack1(ComChrControl * pChrState)
{
	OutputDebugString(L"Stand -> Attack1 ���� ��ȯ\r\n");

	ChrStateAttack1* pStateAttack = new ChrStateAttack1(m_pAnimation);
	pChrState->SetState(pStateAttack);
}

void ChrStateStand::Attack2(ComChrControl * pChrState)
{
}

void ChrStateStand::Attack3(ComChrControl * pChrState)
{
}
