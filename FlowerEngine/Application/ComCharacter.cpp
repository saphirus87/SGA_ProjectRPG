#include "stdafx.h"
#include "ComCharacter.h"
#include "ComObjMap.h"
#include "ComChrControl.h"
#include "ComFollowTarget.h"
#include "ComChrEquipment.h"
#include "ItemInfo.h"
#include "ComEquipment.h"
#include "ComUIInventory.h"

ComCharacter::ComCharacter(CString szName) : 
	Component(szName),
	m_pMap(NULL),
	m_pAnimation(NULL),
	m_pChrEquipment(NULL),
	m_pAttackTarget(NULL),
	m_pFace(NULL),
	m_pUILevel(NULL),
	m_pUIEXP(NULL),
	m_pHPBar(NULL),
	m_pMPBar(NULL),
	m_pComUIDamage(NULL),
	m_pTimerDamage(NULL),
	m_pTimerHPRec(NULL),
	m_pTimerMPRec(NULL),
	m_eType(eChrType_COUNT), // �ʱ�ȭ ������ ���
	m_pAttackHandler(NULL),
	m_pSkill1Handler(NULL),
	IsGroud(false)
{
}

ComCharacter::~ComCharacter()
{
	SAFE_DELETE(m_pAttackHandler);
	SAFE_DELETE(m_pSkill1Handler);
}

void ComCharacter::Awake()
{
	Init();
}

void ComCharacter::Init()
{
	GameObject* pObjMap = GameObject::Find("ObjMap");
	if (pObjMap != NULL)
		m_pMap = (ComObjMap*)pObjMap->GetComponent("ComObjMap");
	if (m_pMap)
	{
		m_pMap->UpdateIndexBufferQuadTree();
		GetHeight();
	}

	// CPP ������
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pChrEquipment = (ComChrEquipment*)gameObject->GetComponent("ComChrEquipment");

	m_pTimerHPRec = new CTimer(CClock::GetInstance()); m_pTimerHPRec->Start();
	m_pTimerMPRec = new CTimer(CClock::GetInstance()); m_pTimerMPRec->Start();
	m_pTimerDamage = new CTimer(CClock::GetInstance()); m_pTimerDamage->Start();

	m_pAttackHandler = new AttackHandler();
	m_pAnimation->pCallbackHandler = m_pAttackHandler;

	m_pSkill1Handler = new Skill1Handler();

	m_pComUIDamage = (ComText3D*)gameObject->GetComponent("ComText3D_Damage");
}

void ComCharacter::GetHeight()
{
	if (m_pMap == NULL)
		return;

	Vector3 pos = gameObject->transform->GetPosition();
	float fHeight = 0.f;
	if (m_pMap->GetHeight(fHeight, pos) == true)
	{
		pos.y = fHeight;
		gameObject->transform->SetPosition(pos);
		IsGroud = true;
	}
}

void ComCharacter::Update()
{
	HPMPRecovery();

	if (m_pTimerDamage->GetTime() > 1.f)
		m_pComUIDamage->Enable = false;
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

			// ���� Ÿ���� ������ �κ��丮�� �ֱ�
			if (pEquip->pItemInfo && m_eType == pEquip->pItemInfo->ChrType)
			{
				// ���� �𵥵常 �κ��丮 ������ ���� -> ĳ���� Ÿ�Ժ��� �κ��丮 ������ ������ ���� �ʿ�
				switch (m_eType)
				{
				case eChrType_Human:
					// �޸��̸� �ٷ� ����
					m_pChrEquipment->Equip(pEquip->pItemInfo);
					collider.gameObject->SetActive(false);
					break;

				case eChrType_Undead:
					ComUIInventory * pInven = (ComUIInventory*)GameObject::Find("InvenUI_Undead")->GetComponent("ComUIInventory");
					pInven->AddItem(pEquip->pItemInfo, 1);
					collider.gameObject->SetActive(false);
					break;
				}
			}
		}
	}
}

void ComCharacter::AttackTarget(ComCharacter * pTarget)
{
	m_pAttackTarget = pTarget;
	LookatTarget();

	// �� ���ݷ� ��� (�� ���ݷ� + ��� ���ݷ�)
	int equipmentDmg = 0;
	if (m_pChrEquipment)
		equipmentDmg = m_pChrEquipment->GetTotalATK_MIN();

	int dmg = Status.ATK_PHY + equipmentDmg;

	pTarget->Defence(dmg);

	// �ٽ� �⺻ �ڵ鷯��
	m_pAnimation->pCallbackHandler = m_pAttackHandler;
}

void ComCharacter::LookatTarget()
{
	if (m_pAttackTarget == NULL)
		return;

	// �÷��̾ �ٶ󺸴� ���� ����
	Vector3 vDir = m_pAttackTarget->gameObject->transform->GetPosition() - gameObject->transform->GetPosition();
	D3DXVec3Normalize(&vDir, &vDir);

	// �ϴ��� Y�����θ� ȸ������
	float angleY = Vector::GetAngleY(&vDir);
	gameObject->transform->SetRotation(0.0f, angleY, 0.0f);
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

	// UI ������ ǥ��
	CString szDmg;
	szDmg.Format(L"%d", dmg);
	if (m_pComUIDamage)
	{
		m_pComUIDamage->SetText(szDmg, Color(1, 0, 0, 1), 0.4f, true);
		m_pComUIDamage->Enable = true;
		m_pTimerDamage->Reset();
	}
	
	// UI ����
	UpdateUI();
}

bool ComCharacter::IsDeath()
{
	if (Status.HP <= 0)
		return true;

	return false;
}

void ComCharacter::LevelUp()
{
	// �������� ó��

	// �̵��ӵ� ����
	Status.MOVE_SPEED += 0.05f;

	// ���� EXP
	Status.EXP = 0;
}

void ComCharacter::HPMPRecovery()
{
	// N�ʸ��� ȸ��
	if (m_pTimerHPRec->GetTime() >= Status.REVTime_HP)
	{
		if (Status.RecoveryHP(Status.REV_HP))
		{
			UpdateUI();
			m_pTimerHPRec->Reset();
		}
	}
	// N�ʸ��� ȸ��
	if (m_pTimerMPRec->GetTime() >= Status.REVTime_MP)
	{
		if (Status.RecoveryMP(Status.REV_MP))
		{
			UpdateUI();
			m_pTimerMPRec->Reset();
		}
	}
}

void ComCharacter::UpdateUI()
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

	if (m_pUILevel && m_pUIEXP)
	{
		CString szLevel;
		szLevel.Format(L"LV:%d\r\n%d/%d", Status.LEVEL, Status.EXP, Status.NextEXP());
		m_pUILevel->SetText(szLevel);

		CString szEXP;
		szEXP.Format(L"EXP:%d/%d", Status.EXP, Status.NextEXP());
		m_pUIEXP->SetText(szEXP);
	}
}

HRESULT AttackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	// Ư�� �����ӿ��� ����
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");

	CString szDebug;
	szDebug.Format(L"AttackHandler Track : %d %s\r\n", Track, pChr->gameObject->Name());
	OutputDebugString(szDebug);

	// �׾ ������
	if (pControl->pAttackTarget == NULL)
		return S_OK;

	pChr->AttackTarget(pControl->pAttackTarget);

	return S_OK;
}

HRESULT Skill1Handler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	// Ư�� �����ӿ��� ����
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");

	CString szDebug;
	szDebug.Format(L"Skill1Handler Track : %d %s\r\n", Track, pChr->gameObject->Name());
	OutputDebugString(szDebug);

	// �׾ ������
	if (pControl->pAttackTarget == NULL)
		return S_OK;

	pChr->AttackSkill1(pControl->pAttackTarget);

	return S_OK;
}
