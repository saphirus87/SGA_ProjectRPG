#include "stdafx.h"
#include "ComHuman.h"
#include "ComCharacter.h"
#include "ComChrControl.h"
#include "ChrStateAttack.h"

ComHuman::ComHuman(CString szName) : 
	ComCharacter(szName),
	uiTextCoolTimeSkill1(NULL)
{
	m_eType = eChrType_Human;
}

ComHuman::~ComHuman()
{
}

void ComHuman::Awake()
{
	Init();

	GameObject* pUIBar = GameObject::Find("testUI");
	if (pUIBar)
	{
		ComDialog* uiDialog = (ComDialog*)pUIBar->GetComponent("ComDialog");

		// HP바
		uiDialog->AddProgressBar(eUI_HPBar_Human, "Resources/ui/6.tga");

		m_pHPBar = uiDialog->GetProgressBar(eUI_HPBar_Human);
		m_pHPBar->SetPosition(Vector3(100, 0, 0));
		m_pHPBar->SetMaxValue(Status.HPMAX);
		UpdateHPBar();

		// MP바
		uiDialog->AddProgressBar(eUI_MPBar_Human, "Resources/ui/6.tga");

		m_pMPBar = uiDialog->GetProgressBar(eUI_MPBar_Human);
		m_pMPBar->SetPosition(Vector3(100, 50, 0));
		m_pMPBar->SetMaxValue(Status.MPMAX);
		m_pMPBar->SetMaxColor(Color(0, 0, 1, 1));
		UpdateHPBar();

		// 스킬 버튼
		float fScreenHeight = DXUTGetWindowHeight();

		uiDialog->AddButton(eUI_SkillBtn1_Human, 
			"Resources/ui/human_skill_1.png", 
			"Resources/ui/human_skill_1_over.png", 
			"Resources/ui/human_skill_1.png", this, "human_skill_1");

		UIButton* btnSkill = uiDialog->GetButton(eUI_SkillBtn1_Human);
		btnSkill->SetPosition(Vector3(50, fScreenHeight - 150.0f, 0.0f));

		uiDialog->AddButton(eUI_SkillBtn2_Human,
			"Resources/ui/human_skill_2.png",
			"Resources/ui/human_skill_2_over.png",
			"Resources/ui/human_skill_2.png", this, "human_skill_2");

		btnSkill = uiDialog->GetButton(eUI_SkillBtn2_Human);
		btnSkill->SetPosition(Vector3(150, fScreenHeight - 150.0f, 0.0f));

		uiDialog->AddButton(eUI_SkillBtn3_Human,
			"Resources/ui/human_skill_3.png",
			"Resources/ui/human_skill_3_over.png",
			"Resources/ui/human_skill_3.png", this, "human_skill_3");

		btnSkill = uiDialog->GetButton(eUI_SkillBtn3_Human);
		btnSkill->SetPosition(Vector3(250, fScreenHeight - 150.0f, 0.0f));

		// 스킬1 쿨타임 텍스트
		uiDialog->AddText(eUI_SkillBtn1_Human_TextCoolTime, Assets::GetFont(Assets::FontType_NORMAL), "3.0");
		if (uiTextCoolTimeSkill1)
		{
			uiTextCoolTimeSkill1 = uiDialog->GetText(eUI_SkillBtn1_Human_TextCoolTime);
			uiTextCoolTimeSkill1->SetPosition(Vector3(50, fScreenHeight - 130.0f, 0.0f));
			uiTextCoolTimeSkill1->SetDrawFormat(DT_CENTER);
		}
	}
	SetAniEvent();
}

void ComHuman::SetAniEvent()
{
	if (m_pAnimation == NULL)
		return;

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
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Skill_2], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f, D3DXPLAY_ONCE);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Skill_1], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f, D3DXPLAY_ONCE);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	vecKeyFrameAnimSet.clear();
}

void ComHuman::Update()
{
	ComCharacter::Update();

	// 코드 리팩토링 필요
	ComChrControl* chrControl = (ComChrControl*)gameObject->GetComponent("ComChrControl");
	ChrStateSkill1* pStateSkill = dynamic_cast<ChrStateSkill1*>(chrControl->m_vecState[eAni_Skill_1]);
	if (pStateSkill)
	{
		if (pStateSkill->m_pTimerCool->GetTime() >= pStateSkill->CoolTime)
		{
			pStateSkill->IsCoolTime = false;
			pStateSkill->m_pTimerCool->Pause(true);
		}

		if (pStateSkill->IsCoolTime == false)
		{
			CString szCoolTime;
			szCoolTime.Format(L"%.1f", pStateSkill->CoolTime);
			if (uiTextCoolTimeSkill1)
				uiTextCoolTimeSkill1->SetText(Assets::GetFont(Assets::FontType_NORMAL), szCoolTime);
		}
		else
		{
			CString szCoolTime;
			szCoolTime.Format(L"%.1f", pStateSkill->CoolTime - pStateSkill->m_pTimerCool->GetTime());
			if (uiTextCoolTimeSkill1)
				uiTextCoolTimeSkill1->SetText(Assets::GetFont(Assets::FontType_NORMAL), szCoolTime);
		}
	}
}

void ComHuman::Render()
{
}

void ComHuman::OnClick(UIButton * pSender)
{
	if (pSender->GetButtonName() == "human_skill_1")
	{
		((ComChrControl*)gameObject->GetComponent("ComChrControl"))->Skill1();
	}
	else if (pSender->GetButtonName() == "human_skill_2")
	{
		((ComChrControl*)gameObject->GetComponent("ComChrControl"))->Skill2();
	}
	else if (pSender->GetButtonName() == "human_skill_3")
	{

	}
}

void ComHuman::OnPress(UIButton * pSender)
{
}