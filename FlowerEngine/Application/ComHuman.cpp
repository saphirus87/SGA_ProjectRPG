#include "stdafx.h"
#include "ComHuman.h"
#include "ComCharacter.h"
#include "ComChrControl.h"
#include "ChrStateAttack.h"
#include "ComChrEquipment.h"
#include "SkillInfo.h"

ComHuman::ComHuman(CString szName) : 
	ComCharacter(szName),
	uiTextCoolTimeSkill1(NULL)
{
	m_eType = eChrType_Human;
}

ComHuman::~ComHuman()
{
	for (auto & s : m_vecSkillInfo)
		SAFE_DELETE(s);
}

void ComHuman::Awake()
{
	Init();
	InitPlayer();

	SetUI();
	SetAniEvent();
	SetSkillInfo();
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

	// Ű �̺�Ʈ �ݹ�
	D3DXKEY_CALLBACK skill1Key;
	skill1Key.pCallbackData = this;
	skill1Key.Time = x;

	// eAni �������� �߰��Ѵ�.
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Skill_2], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f, D3DXPLAY_ONCE);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Skill_1], 1, &skill1Key, D3DXCOMPRESS_DEFAULT, 1.0f, D3DXPLAY_ONCE);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	vecKeyFrameAnimSet.clear();
}

void ComHuman::SetSkillInfo()
{
	// ��ų ���� ���� (���� �Ǵ� �������� �о�� �ϴ� �κ�)
	m_vecSkillInfo.resize(3);

	HumanSkill1* pSkillInfo1 = new HumanSkill1();
	pSkillInfo1->szName = "��ų1";
	pSkillInfo1->UID = 1;
	pSkillInfo1->iAddSkillDmg = 2;
	pSkillInfo1->fRange = 3.0f;
	m_vecSkillInfo[0] = pSkillInfo1;

	SkillInfo* pSkillInfo2 = new SkillInfo();
	pSkillInfo2->szName = "��ų2";
	pSkillInfo2->UID = 2;
	m_vecSkillInfo[1] = pSkillInfo2;

	SkillInfo* pSkillInfo3 = new SkillInfo();
	pSkillInfo3->szName = "��ų3";
	pSkillInfo3->UID = 3;
	m_vecSkillInfo[2] = pSkillInfo3;
}

void ComHuman::SetUI()
{
	GameObject* pUIBar = GameObject::Find("testUI");
	if (pUIBar)
	{
		ComDialog* uiDialog = (ComDialog*)pUIBar->GetComponent("ComDialog");

		m_pComUIDamage = (ComText3D*)gameObject->GetComponent("ComText3D_Damage");

		uiDialog->AddImage(eUI_Human_Img_Face, "./Resources/ui/troll_skill_3_over.png");
		m_pFace = uiDialog->GetImage(eUI_Human_Img_Face);
		m_pFace->SetPosition(Vector3(10, 0, 0));

		// LEVEL �ؽ�Ʈ
		CString szLevel;
		szLevel.Format(L"LV:%d", Status->LEVEL);
		uiDialog->AddText(eUI_Human_Text_LV, Assets::GetFont(Assets::FontType_NORMAL), szLevel);
		m_pUILevel = uiDialog->GetText(eUI_Human_Text_LV);
		if (m_pUILevel)
		{
			m_pUILevel->SetPosition(Vector3(45, 50, 0));
			m_pUILevel->SetDrawFormat(DT_LEFT);
		}

		//EXP �ؽ�Ʈ
		CString szEXP;
		szEXP.Format(L"EXP:%d/%d", Status->EXP, Status->NextEXP());
		uiDialog->AddText(eUI_Human_Text_EXP, Assets::GetFont(Assets::FontType_NORMAL), szEXP);
		m_pUIEXP = uiDialog->GetText(eUI_Human_Text_EXP);
		if (m_pUIEXP)
		{
			m_pUIEXP->SetPosition(Vector3(10, 70, 0));
			m_pUIEXP->SetDrawFormat(DT_LEFT);
		}

		// HP��
		uiDialog->AddProgressBar(eUI_HPBar_Human, "Resources/ui/6.tga");

		m_pHPBar = uiDialog->GetProgressBar(eUI_HPBar_Human);
		m_pHPBar->SetPosition(Vector3(100, 0, 0));
		m_pHPBar->SetMaxValue(Status->HPMAX);
		UpdateUI();

		// MP��
		uiDialog->AddProgressBar(eUI_MPBar_Human, "Resources/ui/6.tga");

		m_pMPBar = uiDialog->GetProgressBar(eUI_MPBar_Human);
		m_pMPBar->SetPosition(Vector3(100, 50, 0));
		m_pMPBar->SetMaxValue(Status->MPMAX);
		m_pMPBar->SetMaxColor(Color(0, 0, 1, 1));
		UpdateUI();

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
		uiTextCoolTimeSkill1 = uiDialog->GetText(eUI_SkillBtn1_Human_TextCoolTime);
		if (uiTextCoolTimeSkill1)
		{
			uiTextCoolTimeSkill1->SetPosition(Vector3(50, fScreenHeight - 130.0f, 0.0f));
			uiTextCoolTimeSkill1->SetDrawFormat(DT_CENTER);
		}
	}
}

void ComHuman::Update()
{
	ComCharacter::Update();

	// �ڵ� �����丵 �ʿ�
	ComChrControl* chrControl = (ComChrControl*)gameObject->GetComponent("ComChrControl");
	ChrStateSkill1* pStateSkill = dynamic_cast<ChrStateSkill1*>(m_vecState[eAni_Skill_1]);
	if (pStateSkill)
	{
		float coolTime = m_vecSkillInfo[0]->fCoolTime;

		if (pStateSkill->m_pTimerCool->GetTime() >= coolTime)
		{
			pStateSkill->IsCoolTime = false;
			pStateSkill->m_pTimerCool->Pause(true);
		}

		if (pStateSkill->IsCoolTime == false)
		{
			CString szCoolTime;
			szCoolTime.Format(L"%.1f", coolTime);
			if (uiTextCoolTimeSkill1)
				uiTextCoolTimeSkill1->SetText(Assets::GetFont(Assets::FontType_NORMAL), szCoolTime);
		}
		else
		{
			CString szCoolTime;
			szCoolTime.Format(L"%.1f", coolTime - pStateSkill->m_pTimerCool->GetTime());
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
		Skill1();
	}
	else if (pSender->GetButtonName() == "human_skill_2")
	{
	}
	else if (pSender->GetButtonName() == "human_skill_3")
	{

	}
}

void ComHuman::OnPress(UIButton * pSender)
{
}

void ComHuman::Skill1()
{
	ComChrControl* pChrControl = (ComChrControl*)gameObject->GetComponent("ComChrControl");

	// ���� ����� �����Ǿ� ���� ������
	if (pAttackTarget == NULL)
	{
		// UI Message: ���� ����� �����Ǿ� ���� �ʽ��ϴ�.
		return;
	}
		
	ChrStateSkill1* pStateSkill = dynamic_cast<ChrStateSkill1*>(m_vecState[eAni_Skill_1]);
	m_pAnimation->pCallbackHandler = m_pSkill1Handler;

	if (pStateSkill->IsCoolTime == true)
	{
		// UI Message : ��Ÿ�� ���Դϴ�.
		return;
	}

	int useMP = m_vecSkillInfo[0]->iUseMP;

	if (Status->MP < useMP)
	{
		// UI Message : MP�� �����մϴ�.
		return;
	}

	SetState(eAni_Skill_1);
	m_pCurrentState->Skill1(eAni_Skill_1);

	// MP ���
	Status->MP -= m_vecSkillInfo[0]->iUseMP;

	// UI ����
	UpdateUI();
}

void ComHuman::Skill2()
{
}

void ComHuman::Skill3()
{
}

void ComHuman::AttackSkill1(ComCharacter * pTarget)
{
	pAttackTarget = pTarget;
	LookatTarget();

	HumanSkill1* pSkill1 = ((HumanSkill1*)m_vecSkillInfo[0]);

	// �� ������Ʈ ������ �Ÿ�
	float fDist = ComTransform::Distance(gameObject, pAttackTarget->gameObject);
	
	// ���� �Ÿ��� �ȵŸ� ������ ���� ����
	if (pSkill1->fRange < fDist)
		return;

	// �� ���ݷ� ��� (�� ���ݷ� + ��� ���ݷ�)
	int equipmentDmg = 0;
	if (m_pChrEquipment)
		equipmentDmg = m_pChrEquipment->GetTotalATK_MIN();

	// ��ų1 ������ ���� = (ĳ���� �⺻ ���ݷ� + ��� ���ݷ�) * 1.5�� + ��ų �߰� ������
	int iDmg = (Status->ATK_PHY + equipmentDmg) * 1.5f + pSkill1->iAddSkillDmg;

	pTarget->Defence(iDmg, false);
}

void ComHuman::AttackSkill2(ComCharacter * pTarget)
{
}

void ComHuman::AttackSkill3(ComCharacter * pTarget)
{
}
