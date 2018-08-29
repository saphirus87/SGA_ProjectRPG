#include "stdafx.h"
#include "ComChrControl.h"
#include "ChrStateAttack.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"

ChrStateAttack1::ChrStateAttack1(ComChrControl* pControl)
{
	m_pControl = pControl;
}

ChrStateAttack1::~ChrStateAttack1()
{
}

void ChrStateAttack1::Stand(int iIndex)
{
	OutputDebugString(L"Attack1 -> Stand ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Stand();
}

void ChrStateAttack1::Walk(int iIndex)
{
	OutputDebugString(L"Attack1 -> Walk ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
}

void ChrStateAttack1::Attack1(int iIndex)
{
	m_pControl->pAnimation->PlayAnimation(iIndex);
}

void ChrStateAttack1::Death(int iIndex)
{
	OutputDebugString(L"Attack1 -> Death ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Death();
}

void ChrStateAttack1::Skill1(int iIndex)
{
	OutputDebugString(L"Attack1 -> Skill1 ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Skill1();
}

void ChrStateAttack1::Skill2(int iIndex)
{
	OutputDebugString(L"Attack1 -> Skill2 ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Skill2();
}

ChrStateSkill1::ChrStateSkill1(ComChrControl * pControl)
{
	m_pControl = pControl;
}

ChrStateSkill1::~ChrStateSkill1()
{
}

void ChrStateSkill1::Stand(int iIndex)
{
	OutputDebugString(L"Skill1 -> Stand ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Stand();
}

void ChrStateSkill1::Walk(int iIndex)
{
}

void ChrStateSkill1::Attack1(int iIndex)
{
}

void ChrStateSkill1::Skill1(int iIndex)
{
	m_pControl->pAnimation->PlayAnimation(iIndex);
}

void ChrStateSkill1::Skill2(int iIndex)
{
}

void ChrStateSkill1::Death(int iIndex)
{
	OutputDebugString(L"Skill1 -> Death ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Death();
}

ChrStateSkill2::ChrStateSkill2(ComChrControl * pControl)
{
	m_pControl = pControl;
}

ChrStateSkill2::~ChrStateSkill2()
{
}

void ChrStateSkill2::Stand(int iIndex)
{
	OutputDebugString(L"Skill2 -> Stand ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Stand();
}

void ChrStateSkill2::Walk(int iIndex)
{
}

void ChrStateSkill2::Attack1(int iIndex)
{
}

void ChrStateSkill2::Skill1(int iIndex)
{
}

void ChrStateSkill2::Skill2(int iIndex)
{
	m_pControl->pAnimation->PlayAnimation(iIndex);
}

void ChrStateSkill2::Death(int iIndex)
{
	OutputDebugString(L"Skill2 -> Death ���� ��ȯ\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Death();
}
