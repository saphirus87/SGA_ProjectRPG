#include "stdafx.h"
#include "ComCharacter.h"


ComCharacter::ComCharacter(CString szName) : 
	Component(szName),
	m_pAnimation(NULL)
{
}


ComCharacter::~ComCharacter()
{
}

void ComCharacter::Awake()
{
	Init();
}

void ComCharacter::Update()
{
}

void ComCharacter::Render()
{
}

void ComCharacter::AttackTarget(ComCharacter * pTarget)
{
	// 총 공격력 계산 (내 공격력 + 장비 공격력)
	int dmg = m_status.ATK_PHY;

	pTarget->Defence(m_status.ATK_PHY);
}

void ComCharacter::Defence(int dmg)
{
	// 총 방어력 계산 (내 방어력 + 장비 방어력)
	int def = m_status.DEF_PHY;

	// HP 차감
	m_status.HP -= dmg;
}

bool ComCharacter::CheckDeath()
{
	if (m_status.HP <= 0)
		return true;

	return false;
}

void ComCharacter::Init()
{
	// CPP 다형성
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
}
