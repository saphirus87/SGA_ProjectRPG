#include "stdafx.h"
#include "ComMissile.h"
#include "ComShooting.h"
#include "ComAircraft.h"

ComMissile::ComMissile(CString szName) : 
	Component(szName),
	m_fFlyLengthMax(0.f),
	m_vDirection(Vector3(0.0f, 0.0f, 1.0f))
{
}

ComMissile::~ComMissile()
{
}

void ComMissile::Awake()
{
	// 이 부분 수정 필요
	GameObject* pAircraft = GameObject::Find("Aircraft");
	m_pAircraft = (ComAircraft*)pAircraft->GetComponent("ComAircraft");
}

void ComMissile::Set(ComShooting * pShooting, float fRange)
{
	m_szShooterName = pShooting->gameObject->Name();
	m_pShooting = pShooting;
	//m_fFlyLengthMax += m_pAircraft->Level() * 2.0f;
	m_fFlyLengthMax = fRange;
}

void ComMissile::Update()
{
	gameObject->transform->Translate(m_vDirection);

	// 날아간 거리 계산
	m_fFlyLength = ComTransform::Distance(gameObject, &m_posStart);

	if (m_fFlyLength > m_fFlyLengthMax)
	{
		// 미사일을 메모리에서 지우지 말고 재사용한다.
		//m_pOwner->Destroy();
		m_pShooting->RemoveMissile(gameObject);
	}
}

void ComMissile::Render()
{
}

void ComMissile::OnTriggerEnter(ComCollider & collider)
{
	// 이 미사일을 발사 한 주인이 적 일 때
	if (m_szShooterName.Find(L"Enermy") >= 0)
	{
		// 미사일이 플레이어 비행기랑 부딛혔을 땐
		if (collider.gameObject->Name().Find(L"Aircraft") >= 0)
			m_pShooting->RemoveMissile(gameObject);
	}
	// 이 미사일을 발사 한 주인이 플레이어 일 때
	else if (m_szShooterName.Find(L"Aircraft") >= 0)
	{
		// 미사일이 적군 비행기랑 부딪혔을 때
		if (collider.gameObject->Name().Find(L"Enermy") >= 0)
		{
			// 미사일을 메모리에서 지우지 말고 재사용한다.
			//m_pOwner->Destroy();
			m_pShooting->RemoveMissile(gameObject);
			m_pAircraft->CheckLevelUp();
			m_pAircraft->CheckGameEnd();
			// 상대 객체 파괴
			collider.gameObject->Destroy();
			//collider.gameObject->SetActive(false);
		}
	}
}


void ComMissile::SetDirection(Vector3 vDir, float fSpeed)
{
	m_vDirection = vDir * fSpeed;
}
