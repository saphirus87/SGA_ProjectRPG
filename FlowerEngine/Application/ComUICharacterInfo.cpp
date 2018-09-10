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
	ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");

	// 장착위치에 맞게 버튼 생성(아이템 칸 마다)
	for (int i = 0; i < 16; ++i)
	{
		CString btnName;
		btnName.Format(L"InvenSlot%d", i + 1);
		uiDialog->AddButton(eEquipUI_HelmetBtn + i, "None", "None", "None", this, btnName);
		uiDialog->GetButton(eEquipUI_HelmetBtn + i)->SetScale(Vector3(0.58f, 0.58f, 0.0f));
		uiDialog->GetButton(eEquipUI_HelmetBtn + i)->SetPosition(Vector3(21.0f + (284.0f * (i / 8)), 74.0f + (41.0f * (i % 8)), 0.0f));
	}
	for (int i = 0; i < 3; ++i)
	{
		CString btnName;
		btnName.Format(L"InvenSlot%d", i + 1);
		uiDialog->AddButton(eEquipUI_MainHandBtn + i, "None", "None", "None", this, btnName);
		uiDialog->GetButton(eEquipUI_MainHandBtn + i)->SetScale(Vector3(0.58f, 0.58f, 0.0f));
		uiDialog->GetButton(eEquipUI_MainHandBtn + i)->SetPosition(Vector3(120.0f + (42.5f * i), 384.5f, 0.0f));
	}


	if (m_pChrEquip)
	{
		ItemInfo* Shoulder = m_pChrEquip->GetEquipedItems()[eRenderEquipment_ShoulderR];
		if (Shoulder)
			uiDialog->GetButton(eInvenUI_Slot1Btn)->SetTexture("Resources/ui/" + Shoulder->IconName, "Resources/ui/" + Shoulder->IconName, "Resources/ui/" + Shoulder->IconName);
	}
}

void ComUICharacterInfo::Update()
{
	UpdateIcons();
}

void ComUICharacterInfo::Render()
{
	
	
}

void ComUICharacterInfo::OnClick(UIButton * pSender)
{
	if (pSender->GetButtonName() == "CharacterInfoClose")
	{
		ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");
		uiDialog->SetIsVisible(false);
	}
}

void ComUICharacterInfo::OnPress(UIButton * pSender)
{
}

void ComUICharacterInfo::UpdateIcons()
{
	return;

	ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");

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

				UIButton* pBtn = uiDialog->GetButton(equipBtnNum);
				if (pBtn)
					pBtn->SetTexture("Resources/ui/" + equip->IconName, "Resources/ui/" + equip->IconName, "Resources/ui/" + equip->IconName);
			}
		}
	}
}