#include "stdafx.h"
#include "ComAircraft.h"
#include "ComAircraftControl.h"
#include "ComMissile.h"
#include "ComFollowTarget.h"
#include "ComUIPanelDialog.h"
#include "ComShooting.h"

ComAircraftControl::ComAircraftControl(CString szName) :
	Component(szName),
	m_isMoving(false),
	m_pos(NULL),
	m_iCurrentSkill2Index(1)
{
}


ComAircraftControl::~ComAircraftControl()
{
}

void ComAircraftControl::Awake()
{
	m_pos = &gameObject->transform->GetPosition();
	m_pAircraft = (ComAircraft*)gameObject->GetComponent("ComAircraft");
	m_pShooting = (ComShooting*)gameObject->GetComponent("ComShooting");
	m_pTimer = new CTimer(CClock::GetInstance());
	m_pTimer->Start();

	GameObject* pGO = GameObject::Find("UIPanelDialog");

	m_pAircraft->CheckGameEnd();
}

void ComAircraftControl::Update()
{
	Move();
	Shooting();
	UseSkill1();
	UseSkill2();

	CString szDebug;
	szDebug.Format(L"MissileCount : %d", ComShooting::MissileCount());
	Debug::Get()->EndLine();
	Debug::Get()->AddText(szDebug);
}

void ComAircraftControl::Render()
{
}

void ComAircraftControl::OnTriggerEnter(ComCollider & collider)
{
	// 내 비행기가 적군이랑 부딪혔을 때

	// 못찾았을 때 -1, 찾았을 때 0~
	int find = collider.gameObject->Name().Find(L"Enermy");

	if (find >= 0)
	{
		m_pAircraft->CheckLevelUp();
		m_pAircraft->CheckGameEnd();

		collider.gameObject->Destroy();
	}
}

void ComAircraftControl::Move()
{
	if (Input::KeyPress('A'))
	{
		gameObject->transform->RotateY(-0.1f);
	}
	if (Input::KeyPress('D'))
	{
		gameObject->transform->RotateY(0.1f);
	}

	float fDeltaZ = 0;
	if (Input::KeyPress('W'))
	{
		m_isMoving = true;
		fDeltaZ = 1;
	}
	else if (Input::KeyPress('S'))
	{
		m_isMoving = true;
		fDeltaZ = -1;
	}
	else
		m_isMoving = false;

	gameObject->transform->GetForward(m_vecForward);
	Vector3 forward = fDeltaZ * m_vecForward * m_pAircraft->MoveSpeed();
	gameObject->transform->Translate(forward);

	//CString szDebug;
	//szDebug.Format(L"forward vec : %f, %f, %f", m_vecForward.x, m_vecForward.y, m_vecForward.z);
	//Debug::Get()->AddText(szDebug);
}

void ComAircraftControl::Shooting()
{
	if (Input::KeyDown(VK_SPACE))
	{
		m_pShooting->CreateMissile(&m_vecForward, 0.5f, m_pAircraft->AttackRange());

		//CreateMissile(&m_vecForward);

		if (m_pAircraft->Level() >= 2)
			m_pShooting->CreateMissile(&Vector::RotateX(&m_vecForward, -45.f), 0.5f, m_pAircraft->AttackRange());
	
		if (m_pAircraft->Level() >= 5)
			m_pShooting->CreateMissile(&Vector::RotateX(&m_vecForward, 45.f), 0.5f, m_pAircraft->AttackRange());
	}
}

void ComAircraftControl::UseSkill1()
{
	//if (m_pAircraft->Level() >= 10)
	{
		if (Input::KeyPress('F'))
		{
			std::list<GameObject*> m_listEnemy;

			list<GameObject*> listEnermy = GameObject::FindAll("Enermy");

			for (auto & go : listEnermy)
			{
				ComFollowTarget* comFollowTarget = (ComFollowTarget*)go->GetComponent("ComFollowTarget");

				float fDist = ComTransform::Distance(go, gameObject);
				if (fDist < 15.0f)
				{
					comFollowTarget->accellation = 0.01f;
				}
			}
		}
	}
}

void ComAircraftControl::UseSkill2()
{
	int iLevel = m_pAircraft->Level();
	int targetLevel = 5; // 5, 6, 7, 8

	// 스킬 레벨 지정
	if (targetLevel <= iLevel && iLevel < targetLevel + 1)
	{
		if (m_pTimer->GetTime() > 0.2f)
		{
			m_pShooting->CreateMissile(&Vector::RotateY(&m_vecForward, m_iCurrentSkill2Index * 30.f), 0.5f, m_pAircraft->AttackRange());
			++m_iCurrentSkill2Index;
			m_pTimer->Reset();

			if (m_iCurrentSkill2Index == 12)
				m_iCurrentSkill2Index = 1;
		}
	}

	if (targetLevel + 1 <= iLevel && iLevel < targetLevel + 2)
	{
		if (m_pTimer->GetTime() > 0.2f)
		{
			m_pShooting->CreateMissile(&Vector::RotateY(&m_vecForward, m_iCurrentSkill2Index * 15.f), 0.5f, m_pAircraft->AttackRange());
			++m_iCurrentSkill2Index;
			m_pTimer->Reset();

			if (m_iCurrentSkill2Index == 24)
				m_iCurrentSkill2Index = 1;
		}
	}

	if (targetLevel + 2 <= iLevel && iLevel < targetLevel + 3)
	{
		if (m_pTimer->GetTime() > 0.1f)
		{
			m_pShooting->CreateMissile(&Vector::RotateY(&m_vecForward, m_iCurrentSkill2Index * 15.f), 0.5f, m_pAircraft->AttackRange());
			++m_iCurrentSkill2Index;
			m_pTimer->Reset();

			if (m_iCurrentSkill2Index == 24)
				m_iCurrentSkill2Index = 1;
		}
	}

	if (targetLevel + 3 <= iLevel  && iLevel < targetLevel + 4)
	{
		if (m_pTimer->GetTime() > 0.1f)
		{
			m_pShooting->CreateMissile(&Vector::RotateY(&m_vecForward, m_iCurrentSkill2Index * 15.f), 0.5f, m_pAircraft->AttackRange());
			m_pShooting->CreateMissile(&Vector::RotateY(&m_vecForward, m_iCurrentSkill2Index * -15.f), 0.5f, m_pAircraft->AttackRange());
			++m_iCurrentSkill2Index;
			m_pTimer->Reset();

			if (m_iCurrentSkill2Index == 24)
				m_iCurrentSkill2Index = 1;
		}
	}

	if (targetLevel + 4 <= iLevel  /*&& iLevel < targetLevel + 5*/)
	{
		if (m_pTimer->GetTime() > 0.1f)
		{
			m_pShooting->CreateMissile(&Vector::RotateY(&m_vecForward, m_iCurrentSkill2Index * 15.f), 0.5f, m_pAircraft->AttackRange());
			m_pShooting->CreateMissile(&Vector::RotateY(&m_vecForward, m_iCurrentSkill2Index * -15.f), 0.5f, m_pAircraft->AttackRange());
			m_pShooting->CreateMissile(&Vector::RotateX(&m_vecForward, m_iCurrentSkill2Index * -15.f), 0.5f, m_pAircraft->AttackRange());
			++m_iCurrentSkill2Index;
			m_pTimer->Reset();

			if (m_iCurrentSkill2Index == 24)
				m_iCurrentSkill2Index = 1;
		}
	}
}

