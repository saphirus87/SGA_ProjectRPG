#include "stdafx.h"
#include "ComTroll.h"
#include "ComObjMap.h"
#include "ComCharacter.h"
#include "ComChrControl.h"

ComTroll::ComTroll(CString szName)
	:ComCharacter(szName)
{
	m_eType = eChrType_Troll;
}

ComTroll::~ComTroll()
{
}

void ComTroll::Awake()
{
	Init();

	GameObject* pUIBar = GameObject::Find("testUI");
	ComDialog* uiDialog = (ComDialog*)pUIBar->GetComponent("ComDialog");

	uiDialog->AddProgressBar(eUI_HPBar_Troll, "Resources/ui/6.tga");

	m_pHPBar = uiDialog->GetProgressBar(eUI_HPBar_Troll);
	m_pHPBar->SetPosition(Vector3(800, 0, 0));
	m_pHPBar->SetMaxValue(Status.HPMAX);

	float fScreenHeight = DXUTGetWindowHeight();
	float fOffsetX = 700;

	uiDialog->AddButton(eUI_SkillBtn1_Troll,
		"Resources/ui/troll_skill_1.png",
		"Resources/ui/troll_skill_1_over.png",
		"Resources/ui/troll_skill_1.png", this, "troll_skill_1");

	UIButton* btnSkill = uiDialog->GetButton(eUI_SkillBtn1_Troll);
	btnSkill->SetPosition(Vector3(fOffsetX + 50, fScreenHeight - 150.0f, 0.0f));

	uiDialog->AddButton(eUI_SkillBtn2_Troll,
		"Resources/ui/troll_skill_2.png",
		"Resources/ui/troll_skill_2_over.png",
		"Resources/ui/troll_skill_2.png", this, "troll_skill_2");

	btnSkill = uiDialog->GetButton(eUI_SkillBtn2_Troll);
	btnSkill->SetPosition(Vector3(fOffsetX + 150, fScreenHeight - 150.0f, 0.0f));

	uiDialog->AddButton(eUI_SkillBtn3_Troll,
		"Resources/ui/troll_skill_3.png",
		"Resources/ui/troll_skill_3_over.png",
		"Resources/ui/troll_skill_3.png", this, "troll_skill_3");

	btnSkill = uiDialog->GetButton(eUI_SkillBtn3_Troll);
	btnSkill->SetPosition(Vector3(fOffsetX + 250, fScreenHeight - 150.0f, 0.0f));

	UpdateUI();

	SetAniEvent();
}

void ComTroll::Update()
{
	ComCharacter::Update();
}

void ComTroll::Render()
{
}

void ComTroll::SetAniEvent()
{
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
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Skill_2], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Skill_1], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	vecKeyFrameAnimSet.clear();
}

void ComTroll::OnClick(UIButton * pSender)
{
	if (pSender->GetButtonName() == "troll_skill_1")
	{
//		((ComChrControl*)gameObject->GetComponent("ComChrControl"))->Attack1();
	}
	else if (pSender->GetButtonName() == "troll_skill_2")
	{
//		((ComChrControl*)gameObject->GetComponent("ComChrControl"))->Stand();
	}
	else if (pSender->GetButtonName() == "troll_skill_3")
	{

	}
}

void ComTroll::OnPress(UIButton * pSender)
{
}
