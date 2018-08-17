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
}

void ComSmallderon::Update()
{
	if (m_pTarget->IsFollowing)
	{
		GetHeight();
	}
	if (m_pTarget->AbleAttack)
	{
		// ���� ���� �Ÿ�

	}

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
	// ���� ���¿��� Stand��
	m_pCurrentState->Stand(eAniMon_Stand);
}

void ComSmallderon::Walk(float fDeltaZ)
{
	// ���� ���¿��� Walk��
	m_pCurrentState->Walk(eAniMon_Walk);
}

void ComSmallderon::Attack1()
{
	// ���� ���¿��� Attack1��
	m_pCurrentState->Attack1(eAniMon_Attack_1);
}
