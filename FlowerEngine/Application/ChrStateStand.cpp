#include "stdafx.h"
#include "ComChrControl.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"

ChrStateStand::ChrStateStand(ComChrControl* pControl)
{
	m_pControl = pControl;
}

ChrStateStand::~ChrStateStand()
{
}

void ChrStateStand::Stand(int iIndex)
{
	m_pControl->m_pAnimation->PlayAnimation(eAni_Stand);
}

void ChrStateStand::Walk(int iIndex)
{
	OutputDebugString(L"Stand -> Walk ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
}

void ChrStateStand::Attack1(int iIndex)
{
	OutputDebugString(L"Stand -> Attack1 ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Attack1();
}
