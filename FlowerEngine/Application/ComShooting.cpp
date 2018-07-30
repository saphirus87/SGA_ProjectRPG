#include "stdafx.h"
#include "ComShooting.h"
#include "ComMissile.h"

std::vector<GameObject*> ComShooting::m_vecMissile;
int ComShooting::MissileIndex = 0;

ComShooting::ComShooting(CString szName) : 
	Component(szName)
{
}


ComShooting::~ComShooting()
{
}

void ComShooting::Awake()
{
}

void ComShooting::Update()
{
	
}

void ComShooting::Render()
{
}

void ComShooting::RemoveMissile(GameObject* pMissile)
{
	std::vector<GameObject*>::iterator iter;
	for (iter = m_vecMissile.begin(); iter != m_vecMissile.end(); ++iter)
	{
		if (pMissile == (*iter))
		{
			//m_vecMissile.erase(iter);
			pMissile->SetActive(false);
			break;
		}
	}
}

void ComShooting::CleanUp()
{
	for (auto & go : m_vecMissile)
		go->Destroy();

	m_vecMissile.clear();
}

void ComShooting::CreateMissile(Vector3 * v, float fSpeed, float fMaxRange)
{
	CString str;
	str.Format(L"Missile%d", MissileIndex++);
	if (MissileIndex >= 1000)
		MissileIndex = 0;

	// 기존에 미사일이 있으면 재사용 한다.
	for (auto & missile : m_vecMissile)
	{
		if (missile->IsActive() == false)
		{
			missile->SetActive(true);
			missile->transform->SetPosition(gameObject->transform->GetPosition());
			ComMissile* pComMissile = static_cast<ComMissile*>(missile->GetComponent("ComMissile"));
			pComMissile->Set(this, fMaxRange);
			pComMissile->SetStartPos(gameObject->transform->GetPosition());
			pComMissile->SetDirection(*v, fSpeed);
			return;
		}
	}

	// 그렇지 않으면 미사일을 생성한다.
	FactoryGameObject factory;
	m_vecMissile.push_back(factory.CreateMissile(str, gameObject, *v, fSpeed, fMaxRange));
}