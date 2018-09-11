#include "stdafx.h"
#include "ComCharacter.h"
#include "ComObjMap.h"
#include "ComChrControl.h"
#include "ComFollowTarget.h"
#include "ComChrEquipment.h"
#include "ItemInfo.h"
#include "ComEquipment.h"
#include "ComUIInventory.h"
#include "ComSmallderonAI.h"
#include "IChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"
#include "ComSmallderonAI.h"

ComCharacter::ComCharacter(CString szName) : 
	Component(szName),
	m_pMap(NULL),
	m_pAnimation(NULL),
	m_pChrEquipment(NULL),
	pAttackTarget(NULL),
	m_pCurrentState(NULL),
	m_pFollow(NULL),
	m_pFace(NULL),
	m_pUILevel(NULL),
	m_pUIEXP(NULL),
	m_pHPBar(NULL),
	m_pMPBar(NULL),
	m_pComUIDamage(NULL),
	m_pComUICritical(NULL),
	m_pTimerDamage(NULL),
	m_pTimerHPRec(NULL),
	m_pTimerMPRec(NULL),
	m_eType(eChrType_COUNT), // 초기화 값으로 사용
	m_pAttackHandler(NULL),
	m_pSkill1Handler(NULL),
	Status(NULL),
	IsGroud(false)
{
}

ComCharacter::~ComCharacter()
{
	SAFE_DELETE(Status);

	for (size_t i = 0; i < m_vecState.size(); ++i)
		SAFE_DELETE(m_vecState[i]);

	SAFE_DELETE(m_pAttackHandler);
	SAFE_DELETE(m_pSkill1Handler);
}

void ComCharacter::Awake()
{
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

	m_pFollow = (ComFollowTarget*)gameObject->GetComponent("ComFollowTarget");
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pChrEquipment = (ComChrEquipment*)gameObject->GetComponent("ComChrEquipment");

	m_pTimerHPRec = new CTimer(CClock::GetInstance()); m_pTimerHPRec->Start();
	m_pTimerMPRec = new CTimer(CClock::GetInstance()); m_pTimerMPRec->Start();
	m_pTimerDamage = new CTimer(CClock::GetInstance()); m_pTimerDamage->Start();
	m_pTimerCritical = new CTimer(CClock::GetInstance()); m_pTimerCritical->Start();

	m_pComUIDamage = (ComText3D*)gameObject->GetComponent("ComText3D_Damage");
	m_pComUICritical = (ComText3D*)gameObject->GetComponent("ComText3D_Critical");
}

void ComCharacter::InitPlayer()
{
	m_pAttackHandler = new AttackHandler();
	m_pSkill1Handler = new Skill1Handler();
	m_pAnimation->pCallbackHandler = m_pAttackHandler;

	m_vecState.resize(eAni_COUNT);
	m_vecState[eAni_Stand] = new ChrStateStand(this);
	m_vecState[eAni_Walk] = new ChrStateWalk(this);
	m_vecState[eAni_Attack_1] = new ChrStateAttack1(this);
	m_vecState[eAni_Skill_1] = new ChrStateSkill1(this);
	m_vecState[eAni_Skill_2] = new ChrStateSkill2(this);

	m_pCurrentState = m_vecState[eAni_Stand];

	Status = new StatusInfo();
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
	if (m_pFollow != NULL && m_pFollow->IsFollowing)
	{
		m_pCurrentState->Walk(eAni_Walk);
		GetHeight();
	}
	else if (m_pFollow != NULL && m_pFollow->AbleAttack)
		Attack1();

	CheckMonDeath();
	m_pCurrentState->Update();

	HPMPRecovery();

	if (m_pTimerDamage->GetTime() > 1.f)
		m_pComUIDamage->Enable = false;

	if (m_pTimerCritical->GetTime() > 1.f)
		m_pComUICritical->Enable = false;
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

			// 장착 타입이 같으면 인벤토리에 넣기
			if (pEquip->pItemInfo && m_eType == pEquip->pItemInfo->ChrType)
			{
				// 현재 언데드만 인벤토리 가지고 있음 -> 캐릭터 타입별로 인벤토리 가지고 있음로 수정 필요
				switch (m_eType)
				{
				case eChrType_Human:
					// 휴먼이면 바로 장착
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
	pAttackTarget = pTarget;
	LookatTarget();

	// 총 공격력 계산 (내 공격력 + 장비 공격력)
	int equipmentDmg = 0;
	if (m_pChrEquipment)
		equipmentDmg = m_pChrEquipment->GetTotalATK_MIN();

	int dmg = Status->ATK_PHY + equipmentDmg;
		
	pTarget->Defence(dmg, Status->IsCritical());

	// 다시 기본 핸들러로
	m_pAnimation->pCallbackHandler = m_pAttackHandler;
}

void ComCharacter::Defence(int Damage, bool bCritical)
{
	// 총 방어력 계산 (내 방어력 + 장비 방어력)
	int equipmentDef = 0;
	if (m_pChrEquipment)
		equipmentDef = m_pChrEquipment->GetTotalDEF_PHY();

	int def = Status->DEF_PHY + equipmentDef;

	Damage -= (def / 2);
	if (bCritical)
		Damage *= 2.0f; // 크리티컬시 2배 데미지

	// HP 차감
	Status->HP -= Damage;

	CString szDmg;
	szDmg.Format(L"%d", Damage);

	if (bCritical)
	{
		// 크리티컬시 노란색 데미지 표시
		m_pComUICritical->SetText(szDmg, Color(1, 1, 0, 1), 0.6f, true);
		m_pComUICritical->Enable = true;
		m_pTimerCritical->Reset();
	}
	else
	{
		// UI 데미지 표시
		m_pComUIDamage->SetText(szDmg, Color(1, 0, 0, 1), 0.4f, true);
		m_pComUIDamage->Enable = true;
		m_pTimerDamage->Reset();
	}

	// UI 갱신
	UpdateUI();
}

void ComCharacter::LookatTarget()
{
	if (pAttackTarget == NULL)
		return;

	// 플레이어를 바라보는 방향 벡터
	Vector3 vDir = pAttackTarget->gameObject->transform->GetPosition() - gameObject->transform->GetPosition();
	D3DXVec3Normalize(&vDir, &vDir);

	// 일단은 Y축으로만 회전하자
	float angleY = Vector::GetAngleY(&vDir);
	gameObject->transform->SetRotation(0.0f, angleY, 0.0f);
}

void ComCharacter::CancleAttackTarget()
{
	pAttackTarget = NULL;
	m_pFollow->IsFollowing = false;
	m_pFollow->AbleAttack = false;
	m_pFollow->pTarget = NULL;
}

void ComCharacter::CheckMonDeath()
{
	// 공격 상대가 죽었으면
	if (pAttackTarget && pAttackTarget->IsDeath() == true)
	{
		// 몬스터 죽음 처리
		pAttackTarget->Death();

		// 캐릭터 레벨업 처리
		if (Status->GetEXPAndCheckLevelUp())
			Status->LevelUP(0.05f);

		CancleAttackTarget();
		Stand();
	}
}

bool ComCharacter::CheckPickingMon()
{
	if (m_pFollow == NULL)
		return false;

	Mouse* pMouse = Input::GetInstance()->m_pMouse;
	Vector3 mousePos = Input::GetInstance()->m_pMouse->GetPosition();

	list<GameObject*> listMonster = GameObject::FindAll(eTag_Monster);

	for (auto & o : listMonster)
	{
		ComRenderCubePN* pCube = (ComRenderCubePN*)o->GetComponent("ComRenderCubePN");

		// 죽었을 때는 몬스터 픽킹을 하지 않는다
		if (pCube->Enable == false)
			continue;

		Ray ray = Ray::RayAtWorldSpace(mousePos.x, mousePos.y);

		vector<Vector3>& vertices = pCube->GetVector();
		for (size_t i = 0; i < vertices.size(); i += 3)
		{
			float dist = 0;
			bool pickMon = ray.CalcIntersectTri(&vertices[i], &dist);

			if (pickMon == true)
			{
				// 몬스터를 따라간다.
				m_pFollow->pTarget = o;
				m_pFollow->fMoveSpeed = Status->MOVE_SPEED;
				pAttackTarget = (ComSmallderonAI*)o->GetComponent("ComCharacter");
				return true;
			}
		}
	}

	return false;
}

bool ComCharacter::IsDeath()
{
	if (Status->HP <= 0)
		return true;

	return false;
}

void ComCharacter::HPMPRecovery()
{
	// N초마다 회복
	if (m_pTimerHPRec->GetTime() >= Status->REVTime_HP)
	{
		if (Status->RecoveryHP(Status->REV_HP))
		{
			UpdateUI();
			m_pTimerHPRec->Reset();
		}
	}
	// N초마다 회복
	if (m_pTimerMPRec->GetTime() >= Status->REVTime_MP)
	{
		if (Status->RecoveryMP(Status->REV_MP))
		{
			UpdateUI();
			m_pTimerMPRec->Reset();
		}
	}
}

void ComCharacter::SetState(int iIndex)
{
	m_pCurrentState = m_vecState[iIndex];
}

void ComCharacter::Stand()
{
	// 현재 상태에서 Stand로
	m_pCurrentState->Stand(eAni_Stand);
}

void ComCharacter::Walk(float fDeltaZ)
{
	// 현재 상태에서 Walk로
	m_pCurrentState->Walk(eAni_Walk);
	GetHeight();

	Vector3 vecForward;
	gameObject->transform->GetForward(vecForward);
	Vector3 forward = fDeltaZ * vecForward * Status->MOVE_SPEED;
	gameObject->transform->Translate(forward);
}

void ComCharacter::Attack1()
{
	// 현재 상태에서 Attack1로
	m_pCurrentState->Attack1(eAni_Attack_1);
}

void ComCharacter::Death()
{
}

void ComCharacter::UpdateUI()
{
	if (m_pHPBar)
	{
		if (Status->HP < 0)
			Status->HP = 0;

		m_pHPBar->SetCurValue(Status->HP);
	}

	if (m_pMPBar)
	{
		if (Status->MP < 0)
			Status->MP = 0;

		m_pMPBar->SetCurValue(Status->MP);
	}

	if (m_pUILevel && m_pUIEXP)
	{
		CString szLevel;
		szLevel.Format(L"LV:%d\r\n%d/%d", Status->LEVEL, Status->EXP, Status->NextEXP());
		m_pUILevel->SetText(szLevel);

		CString szEXP;
		szEXP.Format(L"EXP:%d/%d", Status->EXP, Status->NextEXP());
		m_pUIEXP->SetText(szEXP);
	}
}

HRESULT AttackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	// 특정 프레임에서 공격
	ComCharacter* pChr = (ComCharacter*)pCallbackData;

	CString szDebug;
	szDebug.Format(L"AttackHandler Track : %d %s\r\n", Track, pChr->gameObject->Name());
	OutputDebugString(szDebug);

	// 죽어서 없으면
	if (pChr->pAttackTarget == NULL)
		return S_OK;
	pChr->AttackTarget(pChr->pAttackTarget);

	return S_OK;
}

HRESULT Skill1Handler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	// 특정 프레임에서 공격
	ComCharacter* pChr = (ComCharacter*)pCallbackData;

	CString szDebug;
	szDebug.Format(L"Skill1Handler Track : %d %s\r\n", Track, pChr->gameObject->Name());
	OutputDebugString(szDebug);

	// 죽어서 없으면
	if (pChr->pAttackTarget == NULL)
		return S_OK;

	pChr->AttackSkill1(pChr->pAttackTarget);

	return S_OK;
}

Damage::Damage(int value, float fCritical_PER) :
	Value(value),
	IsCritical(false)
{
	srand((unsigned int)time(NULL));
	int iRandom = rand() % 100;
	if (iRandom < fCritical_PER) // 크리티컬 확률 20%
	{
		IsCritical = true;
	}
}
