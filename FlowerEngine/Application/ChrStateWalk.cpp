#include "stdafx.h"
#include "ComChrControl.h"
#include "ChrStateWalk.h"
#include "ChrStateStand.h"
#include "ChrStateAttack.h"

ChrStateWalk::ChrStateWalk(ComRenderSkinnedMesh* pAnimation)
{
	m_pAnimation = pAnimation;
	m_pAnimation->PlayAnimation(eAni_Walk);
}

ChrStateWalk::~ChrStateWalk()
{
}

void ChrStateWalk::Stand(ComChrControl * pChrState)
{
	OutputDebugString(L"Walk -> Stand ���� ��ȯ\r\n");

	ChrStateStand* pStateStand = new ChrStateStand(m_pAnimation);
	pChrState->SetState(pStateStand);
}

void ChrStateWalk::Walk(ComChrControl * pChrState)
{
	// ������ ����
}

void ChrStateWalk::Attack1(ComChrControl * pChrState)
{
	OutputDebugString(L"Walk -> Attack1 ���� ��ȯ\r\n");

	ChrStateAttack1* pStateAttack = new ChrStateAttack1(m_pAnimation);
	pChrState->SetState(pStateAttack);
}

void ChrStateWalk::Attack2(ComChrControl * pChrState)
{
}

void ChrStateWalk::Attack3(ComChrControl * pChrState)
{
}
