#include "stdafx.h"
#include "ComSmallderon.h"
#include "ComObjMap.h"
#include "ComFollowTarget.h"
#include "IChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"

ComSmallderon::ComSmallderon(CString szName)
	:ComChrControl(szName), 
	m_pTarget(NULL)
{
}

ComSmallderon::~ComSmallderon()
{
}

void ComSmallderon::Awake()
{
	Init();

	m_pTarget = (ComFollowTarget*)gameObject->GetComponent("ComFollowTarget");

	m_vecState.resize(eAniMon_COUNT);
	m_vecState[eAniMon_Stand] = new ChrStateStand(this);
	m_vecState[eAniMon_Walk] = new ChrStateWalk(this);
	m_vecState[eAniMon_Attack_1] = new ChrStateAttack1(this);

	m_pCurrentState = m_vecState[eAniMon_Stand];
}

void ComSmallderon::Update()
{
	if (m_pTarget->IsFollowing)
	{
		GetHeight();
		Walk(1);
	}
	else if (m_pTarget->AbleAttack)
	{
		// 공격 가능 거리
		Attack1();
	}
	else
		Stand();

	//Test Code
	if (Input::KeyDown('1'))
	{
		m_pAnimation->PlayAnimation(eAniMon_Stand);
	}
	if (Input::KeyDown('2'))
	{
		m_pAnimation->PlayAnimation(eAniMon_Walk);
	}
}

void ComSmallderon::Render()
{
}

void ComSmallderon::Stand()
{
	// 현재 상태에서 Stand로
	m_pCurrentState->Stand(eAniMon_Stand);
}

void ComSmallderon::Walk(float fDeltaZ)
{
	// 현재 상태에서 Walk로
	m_pCurrentState->Walk(eAniMon_Walk);
}

void ComSmallderon::Attack1()
{
	// 현재 상태에서 Attack1로
	m_pCurrentState->Attack1(eAniMon_Attack_1);
}
