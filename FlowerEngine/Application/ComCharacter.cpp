#include "stdafx.h"
#include "ComCharacter.h"
#include "ComChrControl.h"

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

	dmg -= def;

	// HP ����
	m_status.HP -= dmg;

	CheckDeath();
}

bool ComCharacter::CheckDeath()
{
	if (m_status.HP <= 0)
	{
		gameObject->SetActive(false);
		return true;
	}

	return false;
}

void ComCharacter::Init()
{
	// CPP ������
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
}


HRESULT AttackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	CString szDebug;
	szDebug.Format(L"EventCallback Track : %d\r\n", Track);
	OutputDebugString(szDebug);

	// Ư�� �����ӿ��� ����
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");
	pChr->AttackTarget(pControl->pAttackTarget);

	return S_OK;
}