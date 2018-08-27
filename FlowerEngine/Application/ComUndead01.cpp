#include "stdafx.h"
#include "ComUndead01.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ComCharacter.h"

ComUndead01::ComUndead01(CString szName)
	:ComCharacter(szName)
{
	m_eType = eChrType_Undead;
}

ComUndead01::~ComUndead01()
{
}

void ComUndead01::Awake()
{
	Init();
	
	GameObject* pUIBar = GameObject::Find("testUI");
	ComDialog* uiDialog = (ComDialog*)pUIBar->GetComponent("ComDialog");

	uiDialog->AddProgressBar(eUI_HPBar_Undead, "Resources/ui/6.tga");

	m_pHPBar = uiDialog->GetProgressBar(eUI_HPBar_Undead);
	m_pHPBar->SetPosition(Vector3(450, 0, 0));
	m_pHPBar->SetMaxValue(Status.HPMAX);
	UpdateHPBar();

	SetAniEvent();
}

void ComUndead01::SetAniEvent()
{
	// 애니메이션 키프레임셋
	vector<LPD3DXKEYFRAMEDANIMATIONSET> vecKeyFrameAnimSet;

	vecKeyFrameAnimSet.resize(eAni_COUNT);

	for (int i = eAni_Attack_3; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->GetAnimationSet(i, (LPD3DXANIMATIONSET*)&vecKeyFrameAnimSet[i]);

	// Register 하는 순서데로 Animation Index가 설정되기 때문에 미리 모두 Unregister 한다.
	for (int i = eAni_Attack_3; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->UnregisterAnimationSet(vecKeyFrameAnimSet[i]);

	// 초당 발생하는 애니메이션 키 프레임 틱의 수를 가져옵니다.
	float fSrcTime = vecKeyFrameAnimSet[eAni_Attack_1]->GetSourceTicksPerSecond();	// 4800

	// eAni_Attack_1 총 프레임 수 : 29
	// eAni_Attack_1 때릴때 애니 프레임 Number : 12
	// 비례식 12 : 29 = x : 4800(SrcTime)
	float x = fSrcTime * 12 / 29;
	
	// 키 이벤트 콜백
	D3DXKEY_CALLBACK attackKey;
	attackKey.pCallbackData = this;
	attackKey.Time = x;

	// eAni 순서데로 추가한다.
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_3], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_2], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	vecKeyFrameAnimSet.clear();
}

void ComUndead01::Update()
{
	ComCharacter::Update();
}

void ComUndead01::Render()
{
}