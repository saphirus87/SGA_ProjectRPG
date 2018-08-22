#include "stdafx.h"
#include "ComCharacter.h"
#include "ComChrControl.h"
#include "ComFollowTarget.h"

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

	// ���� ��밡 �׾�����
	if (pTarget->CheckDeath() == true)
	{
		pTarget->gameObject->SetActive(false);

		ComChrControl* pChrControl = (ComChrControl*)gameObject->GetComponent("ComChrControl");
		pChrControl->pAttackTarget = NULL;

		ComFollowTarget* pFollow = (ComFollowTarget*)(gameObject->GetComponent("ComFollowTarget"));
		pFollow->pTarget = NULL;
	}
}

void ComCharacter::Defence(int dmg)
{
	// �� ���� ��� (�� ���� + ��� ����)
	int def = m_status.DEF_PHY;

	dmg -= def;

	// HP ����
	m_status.HP -= dmg;
	
	// UI ����
	UpdateHPBar();
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


HRESULT AttackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	CString szDebug;
	szDebug.Format(L"EventCallback Track : %d\r\n", Track);
	OutputDebugString(szDebug);

	// Ư�� �����ӿ��� ����
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");

	// �׾ ������
	if (pControl->pAttackTarget == NULL)
		return S_OK;

	pChr->AttackTarget(pControl->pAttackTarget);

	return S_OK;
}