#include "stdafx.h"
#include "ComCharacter.h"
#include "ChrStateAttack.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"

ChrStateAttack1::ChrStateAttack1(ComCharacter* pControl)
{
	m_pControl = pControl;
}

ChrStateAttack1::~ChrStateAttack1()
{
}

void ChrStateAttack1::Stand(int iIndex)
{
	OutputDebugString(L"Attack1 -> Stand 상태 변환\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Stand();
}

void ChrStateAttack1::Walk(int iIndex)
{
	OutputDebugString(L"Attack1 -> Walk 상태 변환\r\n");

	m_pControl->SetState(iIndex);
}

void ChrStateAttack1::Attack1(int iIndex)
{
	m_pControl->m_pAnimation->PlayAnimation(iIndex);
}

void ChrStateAttack1::Death(int iIndex)
{
	OutputDebugString(L"Attack1 -> Death 상태 변환\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Death();
}

void ChrStateAttack1::Skill1(int iIndex)
{
	OutputDebugString(L"Attack1 -> Skill1 상태 변환\r\n");

	m_pControl->SetState(iIndex);
//	m_pControl->Skill1();
}

void ChrStateAttack1::Skill2(int iIndex)
{
	OutputDebugString(L"Attack1 -> Skill2 상태 변환\r\n");

	m_pControl->SetState(iIndex);
//	m_pControl->Skill2();
}

ChrStateSkill1::ChrStateSkill1(ComCharacter * pControl) :
	m_pTimerAnim(NULL),
	IsCoolTime(false)
{
	m_pControl = pControl;
	m_pTimerAnim = new CTimer(CClock::GetInstance());
	m_pTimerCool = new CTimer(CClock::GetInstance());
}

ChrStateSkill1::~ChrStateSkill1()
{
}

void ChrStateSkill1::Update()
{
	AnimationSet anim = NULL;
	m_pControl->m_pAnimation->m_pAniControl->GetTrackAnimationSet(0, &anim);
	if (anim != NULL)
	{
		if (m_pTimerAnim->GetTime() >= anim->GetPeriod())
			m_pControl->Stand();
	}
}

void ChrStateSkill1::Stand(int iIndex)
{
	OutputDebugString(L"Skill1 -> Stand 상태 변환\r\n");

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
	if (IsCoolTime == true)
		return;
	
	m_pControl->m_pAnimation->PlayAnimation(iIndex);
	IsCoolTime = true;
	m_pTimerAnim->Start();
	m_pTimerAnim->Reset();
	m_pTimerCool->Start();
	m_pTimerCool->Reset();
}

void ChrStateSkill1::Skill2(int iIndex)
{
}

void ChrStateSkill1::Death(int iIndex)
{
	OutputDebugString(L"Skill1 -> Death 상태 변환\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Death();
}

ChrStateSkill2::ChrStateSkill2(ComCharacter * pControl)
{
	m_pControl = pControl;
}

ChrStateSkill2::~ChrStateSkill2()
{
}

void ChrStateSkill2::Stand(int iIndex)
{
	OutputDebugString(L"Skill2 -> Stand 상태 변환\r\n");

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
	m_pControl->m_pAnimation->PlayAnimation(iIndex);
}

void ChrStateSkill2::Death(int iIndex)
{
	OutputDebugString(L"Skill2 -> Death 상태 변환\r\n");

	m_pControl->SetState(iIndex);
	m_pControl->Death();
}
