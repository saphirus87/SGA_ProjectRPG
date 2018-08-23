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

	uiDialog->AddProgressBar(eUI_HPBar, "Resources/ui/6.tga");

	m_pHPBar = uiDialog->GetProgressBar(eUI_HPBar);
	m_pHPBar->SetMaxValue(m_status.HPMAX);
	UpdateHPBar();

	// 초당 발생하는 애니메이션 키 프레임 틱의 수를 가져옵니다.
	float fSrcTime = m_vecKeyFrameAnimSet[eAni_Attack_1]->GetSourceTicksPerSecond();	// 4800

	// eAni_Attack_1 총 프레임 수 : 29
	// eAni_Attack_1 때릴때 애니 프레임 Number : 12
	// 비례식 12 : 29 = x : 4800(SrcTime)
	float x = fSrcTime * 12 / 29;
	
	// 키 이벤트 콜백
	D3DXKEY_CALLBACK attackKey;
	attackKey.pCallbackData = this;
	attackKey.Time = x;

	// eAni 순서데로 추가한다.
	m_pAnimation->AddCallbackKeysAndCompress(m_vecKeyFrameAnimSet[eAni_Attack_3], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(m_vecKeyFrameAnimSet[eAni_Attack_2], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(m_vecKeyFrameAnimSet[eAni_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(m_vecKeyFrameAnimSet[eAni_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(m_vecKeyFrameAnimSet[eAni_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	m_vecKeyFrameAnimSet.clear();
}

void ComUndead01::Update()
{
}

void ComUndead01::Render()
{
}

void ComUndead01::UpdateHPBar()
{
	m_pHPBar->SetCurValue(m_status.HP);
}

void ComUndead01::SetAniEvent()
{
}
