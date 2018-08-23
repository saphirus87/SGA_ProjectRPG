#include "stdafx.h"
#include "ComSmallderonAI.h"
#include "ComObjMap.h"
#include "ComFollowTarget.h"
#include "IChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"

ComSmallderonAI::ComSmallderonAI(CString szName)
	:ComChrControl(szName)
{
}

ComSmallderonAI::~ComSmallderonAI()
{
}

void ComSmallderonAI::Awake()
{
	Init();
	
	m_vecState.resize(eAniMon_COUNT);
	m_vecState[eAniMon_Attack_1] = new ChrStateAttack1(this);
	m_vecState[eAniMon_Walk] = new ChrStateWalk(this);
	m_vecState[eAniMon_Stand] = new ChrStateStand(this);

	m_pCurrentState = m_vecState[eAniMon_Stand];

	vector<LPD3DXKEYFRAMEDANIMATIONSET> vecKeyFrameAnimSet;
	vecKeyFrameAnimSet.resize(eAniMon_COUNT);

	for (int i = eAniMon_Attack_1; i < eAniMon_COUNT; ++i)
		pAnimation->m_pAniControl->GetAnimationSet(i, (LPD3DXANIMATIONSET*)&vecKeyFrameAnimSet[i]);

	// Register �ϴ� �������� Animation Index�� �����Ǳ� ������ �̸� ��� Unregister �Ѵ�.
	for (int i = eAniMon_Attack_1; i < eAniMon_COUNT; ++i)
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
	attackKey.pCallbackData = (ComCharacter*)gameObject->GetComponent("ComCharacter");
	attackKey.Time = x;

	// eAni �������� �߰��Ѵ�.
	pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAniMon_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	vecKeyFrameAnimSet.clear();
}

void ComSmallderonAI::Update()
{
	if (m_pFollow->pTarget && m_pFollow->IsFollowing)
	{
		GetHeight();
		Walk(1);
	}
	else if (m_pFollow->pTarget && m_pFollow->AbleAttack)
	{
		// ���� ���� �Ÿ�
		Attack1();
	}
	else
		Stand();

	CheckAttackTargetDeath();
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
