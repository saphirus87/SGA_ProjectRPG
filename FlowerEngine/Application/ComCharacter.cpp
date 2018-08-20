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
	// �� ���ݷ� ��� (�� ���ݷ� + ��� ���ݷ�)
	int dmg = m_status.ATK_PHY;

	pTarget->Defence(m_status.ATK_PHY);
}

void ComCharacter::Defence(int dmg)
{
	// �� ���� ��� (�� ���� + ��� ����)
	int def = m_status.DEF_PHY;

	// HP ����
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
	// CPP ������
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
}
