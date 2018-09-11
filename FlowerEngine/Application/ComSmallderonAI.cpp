#include "stdafx.h"
#include "ComSmallderonAI.h"
#include "ComObjMap.h"
#include "ComFollowTarget.h"
#include "IChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"
#include "SceneRPG.h"

ComSmallderonAI::ComSmallderonAI(CString szName)
	:ComCharacter(szName),
	m_pTimerAttack(NULL)
{
}

ComSmallderonAI::~ComSmallderonAI()
{
}

void ComSmallderonAI::Awake()
{
	Init();
	
	m_vecState.resize(eAniMon_COUNT);
	m_vecState[eAniMon_Death] = new ChrStateDeath(this);
	m_vecState[eAniMon_Attack_1] = new ChrStateAttack1(this);
	m_vecState[eAniMon_Walk] = new ChrStateWalk(this);
	m_vecState[eAniMon_Stand] = new ChrStateStand(this);

	m_pCurrentState = m_vecState[eAniMon_Stand];

	m_pTimerAttack = new CTimer(CClock::GetInstance());
	m_pTimerAttack->Start();

	m_pAttackHandler = new AttackHandler();
	m_pAnimation->pCallbackHandler = m_pAttackHandler;

	vector<LPD3DXKEYFRAMEDANIMATIONSET> vecKeyFrameAnimSet;
	vecKeyFrameAnimSet.resize(eAniMon_COUNT);

	for (int i = eAniMon_Death; i < eAniMon_COUNT; ++i)
		m_pAnimation->m_pAniControl->GetAnimationSet(i, (LPD3DXANIMATIONSET*)&vecKeyFrameAnimSet[i]);

	// Register �ϴ� �������� Animation Index�� �����Ǳ� ������ �̸� ��� Unregister �Ѵ�.
	for (int i = eAniMon_Death; i < eAniMon_COUNT; ++i)
		m_pAnimation->m_pAniControl->UnregisterAnimationSet(vecKeyFrameAnimSet[i]);

	float fPeriod = vecKeyFrameAnimSet[eAniMon_Attack_1]->GetPeriod();
	// �ʴ� �߻��ϴ� �ִϸ��̼� Ű ������ ƽ�� ���� �����ɴϴ�.
	float fSrcTime = vecKeyFrameAnimSet[eAniMon_Attack_1]->GetSourceTicksPerSecond();

	// eAni_Attack_1 �� ������ �� : 29
	// eAni_Attack_1 ������ �ִ� ������ Number : 12
	// ��ʽ� 12 : 29 = x : SrcTime
	float x = fSrcTime * 12 / 29;

	// Ű �̺�Ʈ �ݹ�
	D3DXKEY_CALLBACK attackKey;
	attackKey.pCallbackData = this;
	attackKey.Time = x;

	// eAni �������� �߰��Ѵ�.
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Death], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f, D3DXPLAY_ONCE);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	vecKeyFrameAnimSet.clear();
}

void ComSmallderonAI::Update()
{
	// ���Ͱ� ���� �ʾҰ� ���� ����� ������ ���� ����� ã�´�.
	if (IsDeath() == false && m_pFollow->pTarget == NULL)
		FindAttackTarget();

	if (m_pFollow->pTarget && m_pFollow->IsFollowing)
	{
		m_pFollow->fMoveSpeed = Status->MOVE_SPEED;
		GetHeight();
		Walk(1);
	}
	else if (m_pFollow->pTarget && m_pFollow->AbleAttack)
	{
		// 1�ʿ� �ѹ��� Walk�ϴ°ɷ� �׷��� ������ Walk <-> Attack �Դٰ��� ��
		if (m_pTimerAttack->GetTime() >= 1.0f)
		{
			m_pTimerAttack->Reset();
			// ���� ���� �Ÿ�
			Attack1();
		}
	}
	else
		Stand();

	CheckPlayerDeath();

	if (IsGroud == false)
		GetHeight();
}

void ComSmallderonAI::Render()
{
}

void ComSmallderonAI::Stand()
{
	// ���� ���¿��� Stand��
	m_pCurrentState->Stand(eAniMon_Stand);
}

void ComSmallderonAI::Walk(float fDeltaZ)
{
	// ���� ���¿��� Walk��
	m_pCurrentState->Walk(eAniMon_Walk);
}

void ComSmallderonAI::Attack1()
{
	// ���� ���¿��� Attack1��
	m_pCurrentState->Attack1(eAniMon_Attack_1);
}

void ComSmallderonAI::Death()
{
	m_pComUIDamage->Enable = false;
	m_pComUICritical->Enable = false;

	// ���� ���¿��� Death��
	m_pCurrentState->Death(eAniMon_Death);

	// ������ ���󰡴� ĳ���͸� NULL��
	m_pFollow->pTarget = NULL;

	// �浹�ڽ��� ���� ���� ��󿡼� �����Ѵ�.
	ComRenderCubePN* pCollider = (ComRenderCubePN*)gameObject->GetComponent("ComRenderCubePN");
	pCollider->Enable = false;
}

void ComSmallderonAI::FindAttackTarget()
{
	SceneRPG* sceneRPG = (SceneRPG*)SceneManager::GetInstance()->GetCurrentScene();

	// ���� ������
	if (sceneRPG->IsGameEnd)
		return;

	list<GameObject*> listGO = GameObject::FindAll(eTag_Chracter);
	size_t chrCnt = listGO.size();

	vector<GameObject*> vecChr;
	vecChr.resize(chrCnt);
	int i = 0;
	for (auto & chr : listGO)
		vecChr[i++] = chr;

	int rA = 0, rB = 0;
	GameObject* pTemp = NULL;
	for (int i = 0; i < 20; ++i) // ���÷� 10�� ����
	{
		rA = rand() & 2;
		rB = rand() & 2;
		pTemp = vecChr[rA];
		vecChr[rA] = vecChr[rB];
		vecChr[rB] = pTemp;
	}

	int deathCnt = 0;
	for (auto & chr : vecChr)
	{
		ComCharacter* comChr = (ComCharacter*)chr->GetComponent("ComCharacter");

		if (comChr->IsDeath() == false)
		{
			pAttackTarget = comChr;
			m_pFollow->pTarget = comChr->gameObject;
			break;
		}
		else
			++deathCnt;
	}

	// ĳ���� ��� ����� ���� ����
	if (deathCnt >= chrCnt)
		sceneRPG->IsGameEnd = true;
}

void ComSmallderonAI::CheckPlayerDeath()
{
	// ���� ��밡 �׾�����
	if (pAttackTarget && pAttackTarget->IsDeath() == true)
	{
		// ĳ���� ���� ó��
		if (pAttackTarget->gameObject->Tag == eTag_Chracter)
			pAttackTarget->gameObject->SetActive(false);
	
		// m_pFollow ������ ������ �����Ƿ� ���⿡�� ó��
		CancleAttackTarget();

		// ī�޶� �ٽ� ����
		list<GameObject*> listChr = GameObject::FindAll(eTag_Chracter);
		for (auto & chr : listChr)
		{
			ComCharacter* pComChr = (ComCharacter*)chr->GetComponent("ComCharacter");

			if (pComChr->IsDeath() == false)
				Camera::GetInstance()->SetTarget(&pComChr->gameObject->transform->GetPosition());
		}

		Stand();
	}
}
