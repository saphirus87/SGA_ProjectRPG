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
	// 총 공격력 계산 (내 공격력 + 장비 공격력)
	int dmg = m_status.ATK_PHY;

	pTarget->Defence(m_status.ATK_PHY);

	// 공격 상대가 죽었으면
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
	// 총 방어력 계산 (내 방어력 + 장비 방어력)
	int def = m_status.DEF_PHY;

	dmg -= def;

	// HP 차감
	m_status.HP -= dmg;
	
	// UI 갱신
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
	// CPP 다형성
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
}


HRESULT AttackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	CString szDebug;
	szDebug.Format(L"EventCallback Track : %d\r\n", Track);
	OutputDebugString(szDebug);

	// 특정 프레임에서 공격
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");

	// 죽어서 없으면
	if (pControl->pAttackTarget == NULL)
		return S_OK;

	pChr->AttackTarget(pControl->pAttackTarget);

	return S_OK;
}