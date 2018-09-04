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

		// HP��
		uiDialog->AddProgressBar(eUI_HPBar_Human, "Resources/ui/6.tga");

		m_pHPBar = uiDialog->GetProgressBar(eUI_HPBar_Human);
		m_pHPBar->SetPosition(Vector3(100, 0, 0));
		m_pHPBar->SetMaxValue(Status.HPMAX);
		UpdateHPBar();

		// MP��
		uiDialog->AddProgressBar(eUI_MPBar_Human, "Resources/ui/6.tga");

		m_pMPBar = uiDialog->GetProgressBar(eUI_MPBar_Human);
		m_pMPBar->SetPosition(Vector3(100, 50, 0));
		m_pMPBar->SetMaxValue(Status.MPMAX);
		m_pMPBar->SetMaxColor(Color(0, 0, 1, 1));
		UpdateHPBar();

		// ��ų ��ư
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

		// ��ų1 ��Ÿ�� �ؽ�Ʈ
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

	// �ִϸ��̼� Ű�����Ӽ�
	vector<LPD3DXKEYFRAMEDANIMATIONSET> vecKeyFrameAnimSet;

	vecKeyFrameAnimSet.resize(eAni_COUNT);

	for (int i = eAni_Skill_2; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->GetAnimationSet(i, (LPD3DXANIMATIONSET*)&vecKeyFrameAnimSet[i]);

	// Register �ϴ� �������� Animation Index�� �����Ǳ� ������ �̸� ��� Unregister �Ѵ�.
	for (int i = eAni_Skill_2; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->UnregisterAnimationSet(vecKeyFrameAnimSet[i]);

	// �ʴ� �߻��ϴ� �ִϸ��̼� Ű ������ ƽ�� ���� �����ɴϴ�.
	float fSrcTime = vecKeyFrameAnimSet[eAni_Attack_1]->GetSourceTicksPerSecond();	// 4800

	// eAni_Attack_1 �� ������ �� : 29
	// eAni_Attack_1 ������ �ִ� ������ Number : 12
	// ��ʽ� 12 : 29 = x : 4800(SrcTime)
	float x = fSrcTime * 12 / 29;
	
	// Ű �̺�Ʈ �ݹ�
	D3DXKEY_CALLBACK attackKey;
	attackKey.pCallbackData = this;
	attackKey.Time = x;

	// eAni �������� �߰��Ѵ�.
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

	// �ڵ� �����丵 �ʿ�
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