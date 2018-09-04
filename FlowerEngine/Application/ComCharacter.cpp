#include "stdafx.h"
#include "ComCharacter.h"
#include "ComChrControl.h"
#include "ComFollowTarget.h"
#include "ComChrEquipment.h"
#include "ItemInfo.h"
#include "ComEquipment.h"

ComCharacter::ComCharacter(CString szName) : 
	Component(szName),
	m_pAnimation(NULL),
	m_pChrEquipment(NULL),
	m_pAttackTarget(NULL),
	m_pHPBar(NULL),
	m_pMPBar(NULL),
	m_pTimerHPRec(NULL),
	m_pTimerMPRec(NULL),
	m_eType(eChrType_COUNT) // 초기화 값으로 사용
{
}

ComCharacter::~ComCharacter()
{
}

void ComCharacter::Awake()
{
	Init();
}

void ComCharacter::Init()
{
	// CPP 다형성
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pChrEquipment = (ComChrEquipment*)gameObject->GetComponent("ComChrEquipment");

	m_pTimerHPRec = new CTimer(CClock::GetInstance());
	m_pTimerHPRec->Start();
	m_pTimerMPRec = new CTimer(CClock::GetInstance());
	m_pTimerMPRec->Start();
}

void ComCharacter::Update()
{
	HPMPRecovery();
}

void ComCharacter::Render()
{
}

void ComCharacter::OnTriggerEnter(ComCollider & collider)
{
	if (collider.gameObject->Tag == eTag_Item)
	{
		if (m_pChrEquipment != NULL)
		{
			ComEquipment* pEquip = (ComEquipment*)collider.gameObject->GetComponent("ComEquipment");

			// 장착 타입이 같으면 장착
			if (pEquip->pItemInfo && m_eType == pEquip->pItemInfo->ChrType)
			{
				m_pChrEquipment->Equip(pEquip->pItemInfo);
				collider.gameObject->SetActive(false);
			}
		}
	}
}

void ComCharacter::AttackTarget(ComCharacter * pTarget)
{
	m_pAttackTarget = pTarget;
	ComChrControl* pControl = (ComChrControl*)(gameObject->GetComponent("ComChrControl"));
	pControl->LookatTarget();

	// 총 공격력 계산 (내 공격력 + 장비 공격력)
	int equipmentDmg = 0;
	if (m_pChrEquipment)
		equipmentDmg = m_pChrEquipment->GetTotalATK_MIN();

	int dmg = Status.ATK_PHY + equipmentDmg;

	pTarget->Defence(dmg);
}

void ComCharacter::Defence(int dmg)
{
	// 총 방어력 계산 (내 방어력 + 장비 방어력)
	int equipmentDef = 0;
	if (m_pChrEquipment)
		equipmentDef = m_pChrEquipment->GetTotalDEF_PHY();

	int def = Status.DEF_PHY + equipmentDef;

	dmg -= (def / 2);

	// HP 차감
	Status.HP -= dmg;
	
	// UI 갱신
	UpdateHPMPBar();
}

bool ComCharacter::IsDeath()
{
	if (Status.HP <= 0)
		return true;

	return false;
}

void ComCharacter::HPMPRecovery()
{
	// N초마다 회복
	if (m_pTimerHPRec->GetTime() >= Status.REVTime_HP)
	{
		// 캐릭터가 죽어있지 않을 때와 HP가 꽉차있지 않으면
		if (Status.HP > 0 && Status.HP < Status.HPMAX)
		{
			Status.HP += Status.REV_HP; // 회복양
			UpdateHPMPBar();
		}
		m_pTimerHPRec->Reset();
	}
	// N초마다 회복
	if (m_pTimerMPRec->GetTime() >= Status.REVTime_MP)
	{
		if (Status.MP < Status.MPMAX)
		{
			Status.MP += Status.REV_MP; // 회복양
			UpdateHPMPBar();
		}
		m_pTimerMPRec->Reset();
	}
}

void ComCharacter::UpdateHPMPBar()
{
	if (m_pHPBar)
	{
		if (Status.HP < 0)
			Status.HP = 0;

		m_pHPBar->SetCurValue(Status.HP);
	}

	if (m_pMPBar)
	{
		if (Status.MP < 0)
			Status.MP = 0;

		m_pMPBar->SetCurValue(Status.MP);
	}
}

HRESULT AttackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	// 특정 프레임에서 공격
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");

	CString szDebug;
	szDebug.Format(L"EventCallback Track : %d %s\r\n", Track, pChr->gameObject->Name());
	OutputDebugString(szDebug);

	// 죽어서 없으면
	if (pControl->pAttackTarget == NULL)
		return S_OK;

	pChr->AttackTarget(pControl->pAttackTarget);

	return S_OK;
}