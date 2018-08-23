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
	m_pControl->pAnimation->PlayAnimation(iIndex);
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

void ChrStateStand::Death(int iIndex)
{
	OutputDebugString(L"Stand -> Death ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Death();
}

ChrStateDeath::ChrStateDeath(ComChrControl * pControl)
{
	m_pControl = pControl;
}

ChrStateDeath::~ChrStateDeath()
{
}

void ChrStateDeath::Stand(int iIndex)
{
	// Death���� Stand�� ���� �Ұ�
}

void ChrStateDeath::Walk(int iIndex)
{
	// Death���� Walk�� ���� �Ұ�
}

void ChrStateDeath::Attack1(int iIndex)
{
	// Death���� Attack1�� ���� �Ұ�
}

void ChrStateDeath::Death(int iIndex)
{
	m_pControl->pAnimation->PlayAnimation(iIndex);
}
