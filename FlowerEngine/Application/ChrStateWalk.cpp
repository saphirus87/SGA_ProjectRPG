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

void ChrStateWalk::Attack(ComChrControl * pChrState)
{
	OutputDebugString(L"Walk -> Attack ���� ��ȯ\r\n");

	ChrStateAttack* pStateAttack = new ChrStateAttack(m_pAnimation);
	pChrState->SetState(pStateAttack);
}
