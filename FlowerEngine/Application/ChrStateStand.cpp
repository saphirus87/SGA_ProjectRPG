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
	OutputDebugString(L"Stand -> Walk 상태 변환\r\n");

	m_pControl->SetState(iIndex);
}

void ChrStateStand::Attack1(int iIndex)
{
	OutputDebugString(L"Stand -> Attack1 상태 변환\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Attack1();
}

void ChrStateStand::Death(int iIndex)
{
	OutputDebugString(L"Stand -> Death 상태 변환\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Death();
}

void ChrStateStand::Skill1(int iIndex)
{
	OutputDebugString(L"Stand -> Skill1 상태 변환\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Skill1();
}

void ChrStateStand::Skill2(int iIndex)
{
	OutputDebugString(L"Stand -> Skill2 상태 변환\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Skill2();
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
	// Death에서 Stand로 변경 불가
}

void ChrStateDeath::Walk(int iIndex)
{
	// Death에서 Walk로 변경 불가
}

void ChrStateDeath::Attack1(int iIndex)
{
	// Death에서 Attack1로 변경 불가
}

void ChrStateDeath::Death(int iIndex)
{
	m_pControl->pAnimation->PlayAnimation(iIndex);
}

void ChrStateDeath::Skill1(int iIndex)
{
	// Death에서 Skill1로 변경 불가
}

void ChrStateDeath::Skill2(int iIndex)
{
	// Death에서 Skill2로 변경 불가
}
