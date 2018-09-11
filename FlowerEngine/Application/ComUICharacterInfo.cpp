#include "stdafx.h"
#include "ComUICharacterInfo.h"
#include "ComChrEquipment.h"
#include "ItemInfo.h"


ComUICharacterInfo::ComUICharacterInfo(CString szName)
	: Component(szName), m_pChrEquip(NULL)
{
}


ComUICharacterInfo::~ComUICharacterInfo()
{
}

void ComUICharacterInfo::Awake()
{
	m_pDialog = (ComDialog*)gameObject->GetComponent("ComDialog");

	// 장착위치에 맞게 버튼 생성(아이템 칸 마다)
	for (int i = 0; i < 16; ++i)
	{
		CString btnName;
		btnName.Format(L"InvenSlot%d", i + 1);
		m_pDialog->AddButton(eEquipUI_HelmetBtn + i, "None", "None", "None", this, btnName);
		m_pDialog->GetButton(eEquipUI_HelmetBtn + i)->SetScale(Vector3(0.58f, 0.58f, 0.0f));
		m_pDialog->GetButton(eEquipUI_HelmetBtn + i)->SetPosition(Vector3(21.0f + (284.0f * (i / 8)), 74.0f + (41.0f * (i % 8)), 0.0f));
	}
	for (int i = 0; i < 3; ++i)
	{
		CString btnName;
		btnName.Format(L"InvenSlot%d", i + 1);
		m_pDialog->AddButton(eEquipUI_MainHandBtn + i, "None", "None", "None", this, btnName);
		m_pDialog->GetButton(eEquipUI_MainHandBtn + i)->SetScale(Vector3(0.58f, 0.58f, 0.0f));
		m_pDialog->GetButton(eEquipUI_MainHandBtn + i)->SetPosition(Vector3(120.0f + (42.5f * i), 384.5f, 0.0f));
	}


	if (m_pChrEquip)
	{
		ItemInfo* Shoulder = m_pChrEquip->GetEquipedItems()[eRenderEquipment_ShoulderR];
		if (Shoulder)
			m_pDialog->GetButton(eInvenUI_Slot1Btn)->SetTexture(Shoulder->IconName, Shoulder->IconName, Shoulder->IconName);
	}
}

void ComUICharacterInfo::Update()
{
	UpdateIcons();

	// 테스트 캐릭터 별로 확인
	if (Input::KeyDown(VK_F9))
	{
		m_pChrEquip = (ComChrEquipment*)GameObject::Find("human_01")->GetComponent("ComChrEquipment");
		ClearEquipIcons();
	}
	if (Input::KeyDown(VK_F10))
	{
		m_pChrEquip = (ComChrEquipment*)GameObject::Find("undead_01")->GetComponent("ComChrEquipment");
		ClearEquipIcons();
	}
	if (Input::KeyDown(VK_F11))
	{
		m_pChrEquip = (ComChrEquipment*)GameObject::Find("troll_01")->GetComponent("ComChrEquipment");
		ClearEquipIcons();
	}
}

void ComUICharacterInfo::Render()
{


}

void ComUICharacterInfo::OnClick(UIButton * pSender)
{
	if (pSender->GetButtonName() == "CharacterInfoClose")
	{
		m_pDialog->SetIsVisible(false);
	}
}

void ComUICharacterInfo::OnPress(UIButton * pSender)
{
}

void ComUICharacterInfo::UpdateIcons()
{
	if (m_pChrEquip)
	{
		for (int i = 0; i < m_pChrEquip->GetEquipedItems().size(); ++i)
		{
			ItemInfo* equip = m_pChrEquip->GetEquipedItems()[i];
			if (equip)
			{
				UINT equipBtnNum;
				if (equip->Type == eItem_Shoulder) equipBtnNum = eEquipUI_ShoulderBtn;
				else if (equip->Type == eItem_Helmet) equipBtnNum = eEquipUI_HelmetBtn;
				else if (equip->Type == eItem_Shield) equipBtnNum = eEquipUI_SecondaryHandBtn;
				else if (equip->Type == eItem_WeaponR) equipBtnNum = eEquipUI_MainHandBtn;
				else if (equip->Type == eItem_WeaponL) equipBtnNum = eEquipUI_SecondaryHandBtn;

				m_pDialog->GetButton(equipBtnNum)->SetTexture(equip->IconName, equip->IconName, equip->IconName);
			}
		}
	}
}

void ComUICharacterInfo::ClearEquipIcons()
{
	for (int i = 0; i < eEquipUI_Count; ++i)
	{
		m_pDialog->GetButton(eEquipUI_HelmetBtn + i)->SetTexture("None", "None", "None");
	}
}
