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
	m_eType(eChrType_COUNT) // �ʱ�ȭ ������ ���
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
	// CPP ������
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

			// ���� Ÿ���� ������ ����
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

	// �� ���ݷ� ��� (�� ���ݷ� + ��� ���ݷ�)
	int equipmentDmg = 0;
	if (m_pChrEquipment)
		equipmentDmg = m_pChrEquipment->GetTotalATK_MIN();

	int dmg = Status.ATK_PHY + equipmentDmg;

	pTarget->Defence(dmg);
}

void ComCharacter::Defence(int dmg)
{
	// �� ���� ��� (�� ���� + ��� ����)
	int equipmentDef = 0;
	if (m_pChrEquipment)
		equipmentDef = m_pChrEquipment->GetTotalDEF_PHY();

	int def = Status.DEF_PHY + equipmentDef;

	dmg -= (def / 2);

	// HP ����
	Status.HP -= dmg;
	
	// UI ����
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
	// N�ʸ��� ȸ��
	if (m_pTimerHPRec->GetTime() >= Status.REVTime_HP)
	{
		// ĳ���Ͱ� �׾����� ���� ���� HP�� �������� ������
		if (Status.HP > 0 && Status.HP < Status.HPMAX)
		{
			Status.HP += Status.REV_HP; // ȸ����
			UpdateHPMPBar();
		}
		m_pTimerHPRec->Reset();
	}
	// N�ʸ��� ȸ��
	if (m_pTimerMPRec->GetTime() >= Status.REVTime_MP)
	{
		if (Status.MP < Status.MPMAX)
		{
			Status.MP += Status.REV_MP; // ȸ����
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
	// Ư�� �����ӿ��� ����
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");

	CString szDebug;
	szDebug.Format(L"EventCallback Track : %d %s\r\n", Track, pChr->gameObject->Name());
	OutputDebugString(szDebug);

	// �׾ ������
	if (pControl->pAttackTarget == NULL)
		return S_OK;

	pChr->AttackTarget(pControl->pAttackTarget);

	return S_OK;
}