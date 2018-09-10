#include "stdafx.h"
#include "ComSmallderonAI.h"
#include "ComObjMap.h"
#include "ComFollowTarget.h"
#include "IChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"
#include "ComCharacter.h"
#include "SceneRPG.h"

ComSmallderonAI::ComSmallderonAI(CString szName)
	:ComChrControl(szName),
	m_pTimerAttack(NULL),
	m_pFollow(NULL)
{
}

ComSmallderonAI::~ComSmallderonAI()
{
}

void ComSmallderonAI::Awake()
{
	pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pFollow = (ComFollowTarget*)gameObject->GetComponent("ComFollowTarget");

	m_vecState.resize(eAniMon_COUNT);
	m_vecState[eAniMon_Death] = new ChrStateDeath(this);
	m_vecState[eAniMon_Attack_1] = new ChrStateAttack1(this);
	m_vecState[eAniMon_Walk] = new ChrStateWalk(this);
	m_vecState[eAniMon_Stand] = new ChrStateStand(this);

	m_pCurrentState = m_vecState[eAniMon_Stand];

	m_pTimerAttack = new CTimer(CClock::GetInstance());
	m_pTimerAttack->Start();

	//static bool bOnce = false;

	//if (bOnce == false)
	{
		vector<LPD3DXKEYFRAMEDANIMATIONSET> vecKeyFrameAnimSet;
		vecKeyFrameAnimSet.resize(eAniMon_COUNT);

		for (int i = eAniMon_Death; i < eAniMon_COUNT; ++i)
			pAnimation->m_pAniControl->GetAnimationSet(i, (LPD3DXANIMATIONSET*)&vecKeyFrameAnimSet[i]);

		// Register �ϴ� �������� Animation Index�� �����Ǳ� ������ �̸� ��� Unregister �Ѵ�.
		for (int i = eAniMon_Death; i < eAniMon_COUNT; ++i)
			pAnimation->m_pAniControl->UnregisterAnimationSet(vecKeyFrameAnimSet[i]);

		float fPeriod = vecKeyFrameAnimSet[eAniMon_Attack_1]->GetPeriod();
		// �ʴ� �߻��ϴ� �ִϸ��̼� Ű ������ ƽ�� ���� �����ɴϴ�.
		float fSrcTime = vecKeyFrameAnimSet[eAniMon_Attack_1]->GetSourceTicksPerSecond();

		// eAni_Attack_1 �� ������ �� : 29
		// eAni_Attack_1 ������ �ִ� ������ Number : 12
		// ��ʽ� 12 : 29 = x : SrcTime
		float x = fSrcTime * 12 / 29;

		// Ű �̺�Ʈ �ݹ�
		D3DXKEY_CALLBACK attackKey;
		m_pCharacter = (ComCharacter*)gameObject->GetComponent("ComCharacter");
		attackKey.pCallbackData = m_pCharacter;
		attackKey.Time = x;

		// eAni �������� �߰��Ѵ�.
		pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Death], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f, D3DXPLAY_ONCE);
		pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
		pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
		pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

		vecKeyFrameAnimSet.clear();

		//bOnce = true;
	}
}

void ComSmallderonAI::Update()
{
	// ���Ͱ� ���� �ʾҰ� ���� ����� ������ ���� ����� ã�´�.
	if (m_pCharacter->IsDeath() == false && m_pFollow->pTarget == NULL)
		FindAttackTarget();

	if (m_pFollow->pTarget && m_pFollow->IsFollowing)
	{
		m_pFollow->fMoveSpeed = m_pCharacter->Status.MOVE_SPEED;
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

	CheckAttackTargetDeath();

	if (IsGroud == false)
		GetHeight();
}

void ComSmallderonAI::GetHeight()
{
	GameObject* pObjMap = GameObject::Find("ObjMap");
	ComObjMap* m_pMap = NULL;
	if (pObjMap != NULL)
		m_pMap = (ComObjMap*)pObjMap->GetComponent("ComObjMap");

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
	int deathCnt = 0;
	for (auto & chr : listGO)
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
