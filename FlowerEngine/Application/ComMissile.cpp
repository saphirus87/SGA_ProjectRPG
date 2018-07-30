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
	// �� �κ� ���� �ʿ�
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

	// ���ư� �Ÿ� ���
	m_fFlyLength = ComTransform::Distance(gameObject, &m_posStart);

	if (m_fFlyLength > m_fFlyLengthMax)
	{
		// �̻����� �޸𸮿��� ������ ���� �����Ѵ�.
		//m_pOwner->Destroy();
		m_pShooting->RemoveMissile(gameObject);
	}
}

void ComMissile::Render()
{
}

void ComMissile::OnTriggerEnter(ComCollider & collider)
{
	// �� �̻����� �߻� �� ������ �� �� ��
	if (m_szShooterName.Find(L"Enermy") >= 0)
	{
		// �̻����� �÷��̾� ������ �ε����� ��
		if (collider.gameObject->Name().Find(L"Aircraft") >= 0)
			m_pShooting->RemoveMissile(gameObject);
	}
	// �� �̻����� �߻� �� ������ �÷��̾� �� ��
	else if (m_szShooterName.Find(L"Aircraft") >= 0)
	{
		// �̻����� ���� ������ �ε����� ��
		if (collider.gameObject->Name().Find(L"Enermy") >= 0)
		{
			// �̻����� �޸𸮿��� ������ ���� �����Ѵ�.
			//m_pOwner->Destroy();
			m_pShooting->RemoveMissile(gameObject);
			m_pAircraft->CheckLevelUp();
			m_pAircraft->CheckGameEnd();
			// ��� ��ü �ı�
			collider.gameObject->Destroy();
			//collider.gameObject->SetActive(false);
		}
	}
}


void ComMissile::SetDirection(Vector3 vDir, float fSpeed)
{
	m_vDirection = vDir * fSpeed;
}
