#include "stdafx.h"
#include "ComChrControl.h"
#include "ChrStateWalk.h"
#include "ChrStateStand.h"
#include "ChrStateAttack.h"

ChrStateWalk::ChrStateWalk(ComChrControl* pControl)
{
	m_pControl = pControl;
}

ChrStateWalk::~ChrStateWalk()
{
}

void ChrStateWalk::Stand(int iIndex)
{
	OutputDebugString(L"Walk -> Stand ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Stand();
}

void ChrStateWalk::Walk(int iIndex)
{
	m_pControl->m_pAnimation->PlayAnimation(eAni_Walk);
}

void ChrStateWalk::Attack1(int iIndex)
{
	OutputDebugString(L"Walk -> Attack1 ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Attack1();
}
