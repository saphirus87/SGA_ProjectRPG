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

		// Register 하는 순서데로 Animation Index가 설정되기 때문에 미리 모두 Unregister 한다.
		for (int i = eAniMon_Death; i < eAniMon_COUNT; ++i)
			pAnimation->m_pAniControl->UnregisterAnimationSet(vecKeyFrameAnimSet[i]);

		float fPeriod = vecKeyFrameAnimSet[eAniMon_Attack_1]->GetPeriod();
		// 초당 발생하는 애니메이션 키 프레임 틱의 수를 가져옵니다.
		float fSrcTime = vecKeyFrameAnimSet[eAniMon_Attack_1]->GetSourceTicksPerSecond();

		// eAni_Attack_1 총 프레임 수 : 29
		// eAni_Attack_1 때릴때 애니 프레임 Number : 12
		// 비례식 12 : 29 = x : SrcTime
		float x = fSrcTime * 12 / 29;

		// 키 이벤트 콜백
		D3DXKEY_CALLBACK attackKey;
		m_pCharacter = (ComCharacter*)gameObject->GetComponent("ComCharacter");
		attackKey.pCallbackData = m_pCharacter;
		attackKey.Time = x;

		// eAni 순서데로 추가한다.
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
	// 몬스터가 죽지 않았고 공격 대상이 없으면 공격 대상을 찾는다.
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
		// 1초에 한번씩 Walk하는걸로 그렇지 않으면 Walk <-> Attack 왔다갔다 함
		if (m_pTimerAttack->GetTime() >= 1.0f)
		{
			m_pTimerAttack->Reset();
			// 공격 가능 거리
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
	// 현재 상태에서 Stand로
	m_pCurrentState->Stand(eAniMon_Stand);
}

void ComSmallderonAI::Walk(float fDeltaZ)
{
	// 현재 상태에서 Walk로
	m_pCurrentState->Walk(eAniMon_Walk);
}

void ComSmallderonAI::Attack1()
{
	// 현재 상태에서 Attack1로
	m_pCurrentState->Attack1(eAniMon_Attack_1);
}

void ComSmallderonAI::Death()
{
	// 현재 상태에서 Death로
	m_pCurrentState->Death(eAniMon_Death);

	// 죽으면 따라가는 캐릭터를 NULL로
	m_pFollow->pTarget = NULL;

	// 충돌박스를 꺼서 공격 대상에서 제외한다.
	ComRenderCubePN* pCollider = (ComRenderCubePN*)gameObject->GetComponent("ComRenderCubePN");
	pCollider->Enable = false;
}

void ComSmallderonAI::FindAttackTarget()
{
	SceneRPG* sceneRPG = (SceneRPG*)SceneManager::GetInstance()->GetCurrentScene();

	// 게임 종료라면
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

	// 캐릭터 모두 사망시 게임 종료
	if (deathCnt >= chrCnt)
		sceneRPG->IsGameEnd = true;
}
