#include "stdafx.h"
#include "ComUIInventory.h"
#include "ItemInfo.h"
#include "ComUIItemInfo.h"
#include "UIBtnInvenIcon.h"
#include "ComChrEquipment.h"


ComUIInventory::ComUIInventory(CString szName)
	: Component(szName), m_InvenSize(4), m_Money(0),
	uiDialog(NULL),
	m_pBtnPicked(NULL)
{
}

ComUIInventory::~ComUIInventory()
{
}

void ComUIInventory::Awake()
{
	SetInvenSize(m_InvenSize);

	uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");
	m_pComUIItemInfo = (ComUIItemInfo*)GameObject::Find("ItemInfoUI")->GetComponent("ComUIItemInfo");

	// 인벤토리 이미지와 닫기 버튼 추가
	uiDialog->AddImage(0, "Resources/ui/ui-backpackbackground.png");
	uiDialog->AddButton(2, "Resources/ui/ui-panel-minimizebutton-up.png", "Resources/ui/ui-panel-minimizebutton-up.png",
		"Resources/ui/ui-panel-minimizebutton-down.png", this, "InvenClose");
	//uiDialog->GetButton(2)->SetScale(Vector3(2.5f, 2.5f, 1.0f));
	uiDialog->GetButton(2)->SetPosition(Vector3(230, 8, 0));

	// 인벤토리 크기에 맞게 버튼 생성(아이템 칸 마다)
	for (int i = 0; i < m_InvenSize; ++i)
	{
		CString btnName;
		btnName.Format(L"InvenSlot%d", i + 1);
		uiDialog->AddBtnInvenIcon(eInvenUI_Slot1Btn + i,
			"Resources/ui/icon_sword_1.png", "Resources/ui/icon_sword_1.png", "Resources/ui/icon_sword_1.png", this, btnName);

		// 맵 자료구조에 버튼 넣어둠
		UIBtnInvenIcon* pBtn = (UIBtnInvenIcon*)uiDialog->GetButton(eInvenUI_Slot1Btn + i);
		m_mapButtons.insert(map<CString, UIBtnInvenIcon*>::value_type(btnName, pBtn));

		pBtn->SetScale(Vector3(0.58f, 0.58f, 0.0f));
		pBtn->SetPosition(Vector3(81.5f + (float)(i % 4) * 41.5f, 65.5f + (float)(i / 4) * 41.0f, 0));

		uiDialog->AddText(eInvenUI_Slot1NumText + i, Assets::GetFont(Assets::FontType_NORMAL), "00");

		UIText* pText = uiDialog->GetText(eInvenUI_Slot1NumText + i);
		pText->SetScale(Vector3(0.7f, 0.7f, 0.0f));
		pText->SetPosition(Vector3(81.5f + (float)(i % 4) * 41.5f, 85.5f + (float)(i / 4) * 41.0f, 0));
		pText->SetDrawFormat(DT_RIGHT | DT_VCENTER);
		pText->SetText("");
	}

	// 소지금 표시를 위한 이미지 및 텍스트 추가
	uiDialog->AddImage(eInvenUI_CopperCoinImg, "Resources/ui/ui-coppericon.png");
	uiDialog->AddImage(eInvenUI_SilverCoinImg, "Resources/ui/ui-silvericon.png");
	uiDialog->AddImage(eInvenUI_GoldCoinImg, "Resources/ui/ui-goldicon.png");

	uiDialog->AddText(eInvenUI_CopperCoinText, Assets::GetFont(Assets::FontType_Coin), "00");
	uiDialog->AddText(eInvenUI_SilverCoinText, Assets::GetFont(Assets::FontType_Coin), "00");
	uiDialog->AddText(eInvenUI_GoldCoinText, Assets::GetFont(Assets::FontType_Coin), "000000");

	// 소지금 관련 이미지 텍스트 설정
	uiDialog->GetImage(eInvenUI_CopperCoinImg)->SetPosition(Vector3(226, 229, 0));
	uiDialog->GetText(eInvenUI_CopperCoinText)->SetPosition(Vector3(192, 229, 0));
	uiDialog->GetText(eInvenUI_CopperCoinText)->SetDrawFormat(DT_RIGHT | DT_VCENTER);

	uiDialog->GetImage(eInvenUI_SilverCoinImg)->SetPosition(Vector3(188, 229, 0));
	uiDialog->GetText(eInvenUI_SilverCoinText)->SetPosition(Vector3(154, 229, 0));
	uiDialog->GetText(eInvenUI_SilverCoinText)->SetDrawFormat(DT_RIGHT | DT_VCENTER);

	uiDialog->GetImage(eInvenUI_GoldCoinImg)->SetPosition(Vector3(150, 229, 0));
	uiDialog->GetText(eInvenUI_GoldCoinText)->SetPosition(Vector3(52, 229, 0));
	uiDialog->GetText(eInvenUI_GoldCoinText)->SetDrawFormat(DT_RIGHT | DT_VCENTER);



	// Mouse로 집고 있는 Item 표시를 위한 이미지 및 텍스트 추가(추후 다른곳으로 이동 필요)
	uiDialog->AddImage(eInvenUI_PickedItemImg, "None");
	uiDialog->GetImage(eInvenUI_PickedItemImg)->SetScale(Vector3(0.58f, 0.58f, 0.0f));

	uiDialog->AddText(eInvenUI_PickedItemNumText, Assets::GetFont(Assets::FontType_NORMAL), "00");
	uiDialog->GetText(eInvenUI_PickedItemNumText)->SetScale(Vector3(0.7f, 0.7f, 0.0f));
	uiDialog->GetText(eInvenUI_PickedItemNumText)->SetDrawFormat(DT_RIGHT | DT_VCENTER);
	uiDialog->GetText(eInvenUI_PickedItemNumText)->SetText("");


	// 테스트 코드(테스트 후 삭제)
	// 아이템 생성
	/*for (int i = 0; i < m_InvenSize; ++i)
	{
	m_vecItem[i].first = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X", "Resources/ui/icon_shoulder_1.png");
	m_vecItem[i].second = 20;
	}*/
	// 소지금 설정
	//m_Money = 10101;
	//******************************************************************************************************************************************

	UpdateIcons();
}

void ComUIInventory::Update()
{
	/*uiDialog->GetImage(eInvenUI_PickedItemImg)->SetPosition(Vector3(mousePos.x - 16 - gameObject->transform->GetPosition().x, mousePos.y - 16 - gameObject->transform->GetPosition().y, 0));
	uiDialog->GetText(eInvenUI_PickedItemNumText)->SetPosition(Vector3(mousePos.x - 16 - gameObject->transform->GetPosition().x, mousePos.y + 4 - gameObject->transform->GetPosition().y, 0));
*/

// 선택된 아이템이 있을 때 위치 이동
	if (m_pBtnPicked && m_pBtnPicked->pItem)
	{
		Vector2 mousePos = Input::GetMousePosition();
		UIImage* pImage = uiDialog->GetImage(eInvenUI_PickedItemImg);
		pImage->SetTexture(m_pBtnPicked->pItem->IconName);
		pImage->SetPosition(Vector3(mousePos.x - 16 - gameObject->transform->GetPosition().x, mousePos.y - 16 - gameObject->transform->GetPosition().y, 0));
	}
}

void ComUIInventory::Render()
{
	for (auto & btn : m_mapButtons)
		btn.second->Render();
}

void ComUIInventory::OnClick(UIButton* pSender)
{
	// 인벤토리 닫기
	if (pSender->GetButtonName() == "InvenClose")
	{
		ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");
		uiDialog->SetIsVisible(false);
		return;
	}

	//if (m_PickedItem.first == nullptr)
	//{

	//}
	//else
	//{
	//	// 인벤토리 내에서 아이콘끼리 위치 변경
	//	CString slotName;
	//	for (int i = 0; i < m_InvenSize; i++)
	//	{
	//		slotName.Format(L"InvenSlot%d", i + 1);
	//		if (pSender->GetButtonName() == slotName)
	//		{
	//			m_PickedItem = InsertItemToSlot(m_PickedItem.first, m_PickedItem.second, i);

	//			UpdateIcons();
	//			break;
	//		}
	//	}
	//}

	// 송현국

	// 이동중인 아이템이 있으면 처리
	if (m_pBtnPicked)
	{
		// 이동중인 아이템이 슬롯 영역에 있는가? 체크

		for (auto & btn : m_mapButtons)
		{
		}

		return;
	}

	UIBtnInvenIcon* pBtn = m_mapButtons[pSender->GetButtonName()];

	m_pComUIItemInfo->SetPosition(Vector3(gameObject->transform->GetPosition().x + 300.0f, gameObject->transform->GetPosition().y, 0.0f));
	m_pComUIItemInfo->SetIsVisible(true);
	m_pComUIItemInfo->SetItemInfo(pBtn->pItem);


	// 아이템 장착 부분
	/*UIBtnInvenIcon* pBtn = m_mapButtons[pSender->GetButtonName()];
	if (pBtn->pItem == NULL)
		return;

	// STEP2. TODO : 인벤토리에서 해당 아이템을 클릭했을 때 판매UI를 출력

	// STEP3. 확인 버튼을 클릭했을 때 해당 캐릭터에 장비 장착, 취소 버튼 눌렀을 때는 아무 동작 안함
	// 3-1. 캐릭터 타입으로 캐릭터를 찾는다. (ComChrEquipment)
	switch (pBtn->pItem->ChrType)
	{
	case eChrType_Undead:
		ComChrEquipment* pChrEquipment = (ComChrEquipment*)GameObject::Find("undead_01")->GetComponent("ComChrEquipment");
		pChrEquipment->Equip(pBtn->pItem);

		// 해당 버튼은 아이템 없음
		pBtn->pItem = NULL;
		break;
	}*/
}

void ComUIInventory::OnPress(UIButton * pSender)
{
	/*CString slotName;
	for (int i = 0; i < m_InvenSize; i++)
	{
		slotName.Format(L"InvenSlot%d", i + 1);
		if (pSender->GetButtonName() == slotName)
		{
			PickItem(i);
			UpdateIcons();
			break;
		}
	}*/

	m_pBtnPicked = m_mapButtons[pSender->GetButtonName()];
	m_pBtnPicked->SetIsVisible(false);
}

bool ComUIInventory::AddItem(ItemInfo* Item, UINT ItemNum)
{
	//int ItemIndex = 0;

	//// 해당 아이템이 없으면
	//if (false == FindItem(Item->UID, ItemIndex))
	//{
	//	ItemIndex = 0;
	//	// 빈슬롯을 찾아서 없으면
	//	if (false == FindEmptySlot(ItemIndex))
	//	{
	//		// UI Message : 인벤토리가 꽉 찼습니다.
	//		return make_pair(Item, ItemNum);
	//	}
	//	// 빈슬롯이 있으면
	//	else
	//	{
	//		m_vecItem[ItemIndex] = make_pair(Item, ItemNum);
	//		// UI 업데이트
	//		UpdateIcons();
	//		return make_pair(nullptr, 0);
	//	}
	//}
	//else
	//{
	//	do
	//	{
	//		m_vecItem[ItemIndex].second += ItemNum;

	//		if (m_vecItem[ItemIndex].second > ItemMaxNum)
	//		{
	//			ItemNum = m_vecItem[ItemIndex].second - ItemMaxNum;
	//			m_vecItem[ItemIndex].second = ItemMaxNum;
	//		}
	//		else
	//		{
	//			ItemNum = 0;
	//			return make_pair(nullptr, 0);
	//		}
	//	} while (FindItem(Item->UID, ItemIndex) && ItemNum > 0);

	//	ItemIndex = 0;
	//	if (!FindEmptySlot(ItemIndex)) return make_pair(Item, ItemNum);
	//	else
	//	{
	//		m_vecItem[ItemIndex] = make_pair(Item, ItemNum);
	//		return make_pair(nullptr, 0);
	//	}
	//}

	//return make_pair(Item, ItemNum);



	if (m_mapButtons.size() > m_InvenSize)
	{
		// UI Message : 인벤토리가 꽉 찼습니다.
		return false;
	}

	for (auto & btn : m_mapButtons)
	{
		if (btn.second->pItem == NULL)
		{
			btn.second->pItem = Item;
			break;
		}
	}
}

pair<ItemInfo*, UINT> ComUIInventory::InsertItemToSlot(ItemInfo * Item, UINT ItemNum, int InvenSlot)
{
	/*if (m_vecItem[InvenSlot].first == nullptr)
	{
		m_vecItem[InvenSlot] = make_pair(Item, ItemNum);

		return make_pair(nullptr, 0);
	}

	if (Item->UID == m_vecItem[InvenSlot].first->UID && m_vecItem[InvenSlot].second < ItemMaxNum)
	{
		if (ItemNum + m_vecItem[InvenSlot].second < ItemMaxNum)
		{
			m_vecItem[InvenSlot].second += ItemNum;

			return make_pair(nullptr, 0);
		}
		else
		{
			ItemNum += m_vecItem[InvenSlot].second;
			ItemNum -= ItemMaxNum;
			m_vecItem[InvenSlot].second = ItemMaxNum;

			return make_pair(Item, ItemNum);
		}
	}
	else
	{
		pair<ItemInfo*, UINT> invenItem = m_vecItem[InvenSlot];
		m_vecItem[InvenSlot] = make_pair(Item, ItemNum);

		return invenItem;
	}*/

	return make_pair(Item, ItemNum);
}

pair<ItemInfo*, UINT> ComUIInventory::DeleteItem(int InvenSlot, UINT ItemNum)
{
	/*if (InvenSlot < m_vecItem.size())
	{
		pair<ItemInfo*, UINT> DeletedItem;

		if (ItemNum == 0 || m_vecItem[InvenSlot].second == ItemNum)
		{
			DeletedItem = m_vecItem[InvenSlot];
			m_vecItem[InvenSlot] = make_pair(nullptr, 0);
		}
		else if (m_vecItem[InvenSlot].second < ItemNum)
		{
			DeletedItem.first = m_vecItem[InvenSlot].first;
			DeletedItem.second = ItemNum;
			m_vecItem[InvenSlot].second -= ItemNum;
		}

		return DeletedItem;
	}
	*/
	return make_pair(nullptr, 0);
}

bool ComUIInventory::FindItem(unsigned int ItemID, int& StartIndex)
{
	/*for (; StartIndex < m_vecItem.size(); ++StartIndex)
	{
		if (m_vecItem[StartIndex].first == NULL)
			continue;
		if (m_vecItem[StartIndex].first->UID == ItemID)
			return true;
	}
	*/
	return false;
}

bool ComUIInventory::FindEmptySlot(int& StartIndex)
{
	/*for (; StartIndex < m_vecItem.size(); ++StartIndex)
	{
		if (m_vecItem[StartIndex].first == nullptr) return true;
	}
	*/
	return false;
}

bool ComUIInventory::PickItem(int InvenSlot)
{
	m_PickedItem = DeleteItem(InvenSlot, 0);

	return true;
}

void ComUIInventory::EarnMoney(UINT money)
{
	m_Money += money;
	UpdateIcons();
}

bool ComUIInventory::SpendMoney(UINT money)
{
	if (m_Money < money) return false;

	m_Money -= money;
	UpdateIcons();
	return false;
}

void ComUIInventory::UpdateIcons()
{
	//// Inventory Slot 이미지 및 텍스트 업데이트
	//for (int i = 0; i < m_vecItem.size(); i++)
	//{
	//	// 텍스쳐 변경 부분
	//	if (m_vecItem[i].first == nullptr)
	//		uiDialog->GetButton(eInvenUI_Slot1Btn + i)->SetTexture("None", "None", "None");
	//	else
	//		uiDialog->GetButton(eInvenUI_Slot1Btn + i)->SetTexture(m_vecItem[i].first->IconName, m_vecItem[i].first->IconName, m_vecItem[i].first->IconName);

	//	if (m_vecItem[i].second <= 1)
	//		uiDialog->GetText(eInvenUI_Slot1NumText + i)->SetText("");
	//	else
	//	{
	//		CString num;
	//		num.Format(L"%d", m_vecItem[i].second);
	//		uiDialog->GetText(eInvenUI_Slot1NumText + i)->SetText(num);
	//	}
	//}

	// Mouse로 집고 있는 Item 이미지 및 텍스트 업데이트
	if (m_PickedItem.first == nullptr)
		uiDialog->GetImage(eInvenUI_PickedItemImg)->SetTexture("None");
	else
		uiDialog->GetImage(eInvenUI_PickedItemImg)->SetTexture(m_PickedItem.first->IconName);

	if (m_PickedItem.second <= 1)
		uiDialog->GetText(eInvenUI_PickedItemNumText)->SetText("");
	else
	{
		CString num;
		num.Format(L"%d", m_PickedItem.second);
		uiDialog->GetText(eInvenUI_PickedItemNumText)->SetText(num);
	}

	// 소지금 이미지 및 텍스트 업데이트
	if (m_Money < 10000)
	{
		uiDialog->GetImage(eInvenUI_GoldCoinImg)->SetIsVisible(false);
		uiDialog->GetText(eInvenUI_GoldCoinText)->SetIsVisible(false);
	}

	if (m_Money < 100)
	{
		uiDialog->GetImage(eInvenUI_SilverCoinImg)->SetIsVisible(false);
		uiDialog->GetText(eInvenUI_SilverCoinText)->SetIsVisible(false);
	}

	if (m_Money > 10000)
	{
		CString gold;
		gold.Format(L"%d", m_Money / 10000);
		uiDialog->GetText(eInvenUI_GoldCoinText)->SetText(gold);

		uiDialog->GetImage(eInvenUI_GoldCoinImg)->SetIsVisible(true);
		uiDialog->GetText(eInvenUI_GoldCoinText)->SetIsVisible(true);
	}
	if (m_Money > 100)
	{
		CString silver;
		silver.Format(L"%d", (m_Money / 100) % 100);
		uiDialog->GetText(eInvenUI_SilverCoinText)->SetText(silver);

		uiDialog->GetImage(eInvenUI_SilverCoinImg)->SetIsVisible(true);
		uiDialog->GetText(eInvenUI_SilverCoinText)->SetIsVisible(true);
	}

	CString copper;
	copper.Format(L"%d", m_Money % 100);
	uiDialog->GetText(eInvenUI_CopperCoinText)->SetText(copper);
}

bool ComUIInventory::SetInvenSize(UINT InvenSize)
{
	/*if (m_InvenSize <= InvenSize)
	{
		m_vecItem.resize(InvenSize, make_pair(nullptr, 0));
		m_InvenSize = InvenSize;

		return true;
	}
	else
	{
		for (int i = InvenSize; i < m_InvenSize; ++i)
		{
			if (m_vecItem[i].first != nullptr) return false;
		}

		m_vecItem.resize(InvenSize);
		m_InvenSize = InvenSize;

		return true;
	}*/

	return true;
}