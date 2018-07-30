#include "stdafx.h"
#include "ComAircraftEnermyAI.h"
#include "ComShooting.h"
#include "ComFollowTarget.h"

ComAircraftEnermyAI::ComAircraftEnermyAI(CString szName) : 
	Component(szName),
	m_pShooting(NULL),
	m_pFollowTarget(NULL)
{
}

ComAircraftEnermyAI::~ComAircraftEnermyAI()
{
	// �� �ı� �� �� �̻��� ��� �ı�
	//m_pShooting->CleanUp();
}

void ComAircraftEnermyAI::Awake()
{
	m_pShooting = (ComShooting*)gameObject->GetComponent("ComShooting");
	m_pFollowTarget = (ComFollowTarget*)gameObject->GetComponent("ComFollowTarget");
	m_pTimer = new CTimer(CClock::GetInstance());
	m_pTimer->Start();
}

void ComAircraftEnermyAI::Update()
{
	// ���󰡴� �� �϶���
	if (m_pFollowTarget->IsFollowing == true)
	{
		if (m_pTimer->GetTime() > 2.5f)
		{
			GameObject* pPlayer = GameObject::Find("Aircraft");
			Vector3 vDir = m_pFollowTarget->pTarget->transform->GetPosition() - gameObject->transform->GetPosition();
			D3DXVec3Normalize(&vDir, &vDir);
			m_pShooting->CreateMissile(&vDir, 0.05f, 20.f);

			m_pTimer->Reset();
		}
	}
}

void ComAircraftEnermyAI::Render()
{
}
