#include "stdafx.h"
#include "ComUndead.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ComCharacter.h"

ComUndead::ComUndead(CString szName)
	:ComCharacter(szName)
{
	m_eType = eChrType_Undead;
}

ComUndead::~ComUndead()
{
}

void ComUndead::Awake()
{
	Init();
	{
		GameObject* pUIBar = GameObject::Find("testUI");
		ComDialog* uiDialog = (ComDialog*)pUIBar->GetComponent("ComDialog");

		uiDialog->AddProgressBar(eUI_HPBar_Undead, "Resources/ui/6.tga");

		m_pHPBar = uiDialog->GetProgressBar(eUI_HPBar_Undead);
		m_pHPBar->SetPosition(Vector3(450, 0, 0));
		m_pHPBar->SetMaxValue(Status.HPMAX);
		UpdateHPMPBar();

		// 스킬 버튼
		float fScreenHeight = DXUTGetWindowHeight();
		float fOffsetX = 350;

		uiDialog->AddButton(eUI_SkillBtn1_Undead,
			"Resources/ui/undead_skill_1.png",
			"Resources/ui/undead_skill_1_over.png",
			"Resources/ui/undead_skill_1.png", NULL, "undead_skill_1");

		UIButton* btnSkill = uiDialog->GetButton(eUI_SkillBtn1_Undead);
		btnSkill->SetPosition(Vector3(fOffsetX + 50, fScreenHeight - 150.0f, 0.0f));

		uiDialog->AddButton(eUI_SkillBtn2_Undead,
			"Resources/ui/undead_skill_2.png",
			"Resources/ui/undead_skill_2_over.png",
			"Resources/ui/undead_skill_2.png", NULL, "undead_skill_2");

		btnSkill = uiDialog->GetButton(eUI_SkillBtn2_Undead);
		btnSkill->SetPosition(Vector3(fOffsetX + 150, fScreenHeight - 150.0f, 0.0f));

		uiDialog->AddButton(eUI_SkillBtn3_Undead,
			"Resources/ui/undead_skill_3.png",
			"Resources/ui/undead_skill_3_over.png",
			"Resources/ui/undead_skill_3.png", NULL, "undead_skill_3");

		btnSkill = uiDialog->GetButton(eUI_SkillBtn3_Undead);
		btnSkill->SetPosition(Vector3(fOffsetX + 250, fScreenHeight - 150.0f, 0.0f));
	}
	SetAniEvent();
}

void ComUndead::SetAniEvent()
{
	// 애니메이션 키프레임셋
	vector<LPD3DXKEYFRAMEDANIMATIONSET> vecKeyFrameAnimSet;

	vecKeyFrameAnimSet.resize(eAni_COUNT);

	for (int i = eAni_Skill_2; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->GetAnimationSet(i, (LPD3DXANIMATIONSET*)&vecKeyFrameAnimSet[i]);

	// Register 하는 순서데로 Animation Index가 설정되기 때문에 미리 모두 Unregister 한다.
	for (int i = eAni_Skill_2; i < eAni_COUNT; ++i)
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
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Skill_2], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Skill_1], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	vecKeyFrameAnimSet.clear();
}

void ComUndead::Update()
{
	ComCharacter::Update();
}

void ComUndead::Render()
{
}