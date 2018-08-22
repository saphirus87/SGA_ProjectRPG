#include "stdafx.h"
#include "ComSmallderon.h"
#include "ComObjMap.h"
#include "ComFollowTarget.h"
#include "IChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"

ComSmallderon::ComSmallderon(CString szName)
	:ComChrControl(szName)
{
}

ComSmallderon::~ComSmallderon()
{
}

void ComSmallderon::Awake()
{
	Init();
	
	m_vecState.resize(eAniMon_COUNT);
	m_vecState[eAniMon_Attack_1] = new ChrStateAttack1(this);
	m_vecState[eAniMon_Walk] = new ChrStateWalk(this);
	m_vecState[eAniMon_Stand] = new ChrStateStand(this);

	m_pCurrentState = m_vecState[eAniMon_Stand];

	//vector<LPD3DXKEYFRAMEDANIMATIONSET> vecKeyFrameAnimSet;
	//vecKeyFrameAnimSet.resize(eAniMon_COUNT);

	//for (int i = eAniMon_Attack_1; i < eAniMon_COUNT; ++i)
	//	pAnimation->m_pAniControl->GetAnimationSet(i, (LPD3DXANIMATIONSET*)&vecKeyFrameAnimSet[i]);

	//// Register 하는 순서데로 Animation Index가 설정되기 때문에 미리 모두 Unregister 한다.
	//for (int i = eAniMon_Attack_1; i < eAniMon_COUNT; ++i)
	//	pAnimation->m_pAniControl->UnregisterAnimationSet(vecKeyFrameAnimSet[i]);

	//float fPeriod = vecKeyFrameAnimSet[eAniMon_Attack_1]->GetPeriod();
	//// 초당 발생하는 애니메이션 키 프레임 틱의 수를 가져옵니다.
	//float fSrcTime = vecKeyFrameAnimSet[eAniMon_Attack_1]->GetSourceTicksPerSecond();

	//// eAni_Attack_1 총 프레임 수 : 29
	//// eAni_Attack_1 때릴때 애니 프레임 Number : 12
	//// 비례식 12 : 29 = x : SrcTime
	//float x = fSrcTime * 12 / 29;

	//// 키 이벤트 콜백
	//D3DXKEY_CALLBACK attackKey;
	//attackKey.pCallbackData = this;
	//attackKey.Time = x;

	//// eAni 순서데로 추가한다.
	//pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	//pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	//pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	//vecKeyFrameAnimSet.clear();
}

void ComSmallderon::Update()
{
	if (m_pTarget->IsFollowing)
	{
		GetHeight();
		Walk(1);
	}
	else if (m_pTarget->AbleAttack)
	{
		// 공격 가능 거리
		Attack1();
	}
	else
		Stand();
}

void ComSmallderon::Render()
{
}

void ComSmallderon::Stand()
{
	// 현재 상태에서 Stand로
	m_pCurrentState->Stand(eAniMon_Stand);
}

void ComSmallderon::Walk(float fDeltaZ)
{
	// 현재 상태에서 Walk로
	m_pCurrentState->Walk(eAniMon_Walk);
}

void ComSmallderon::Attack1()
{
	// 현재 상태에서 Attack1로
	m_pCurrentState->Attack1(eAniMon_Attack_1);
}
