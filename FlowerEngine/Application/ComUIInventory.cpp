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

	// �κ��丮 �̹����� �ݱ� ��ư �߰�
	uiDialog->AddImage(0, "Resources/ui/ui-backpackbackground.png");
	uiDialog->AddButton(2, "Resources/ui/ui-panel-minimizebutton-up.png", "Resources/ui/ui-panel-minimizebutton-up.png",
		"Resources/ui/ui-panel-minimizebutton-down.png", this, "InvenClose");
	//uiDialog->GetButton(2)->SetScale(Vector3(2.5f, 2.5f, 1.0f));
	uiDialog->GetButton(2)->SetPosition(Vector3(230, 8, 0));

	// �κ��丮 ũ�⿡ �°� ��ư ����(������ ĭ ����)
	for (int i = 0; i < m_InvenSize; ++i)
	{
		CString btnName;
		btnName.Format(L"InvenSlot%d", i + 1);
		uiDialog->AddBtnInvenIcon(eInvenUI_Slot1Btn + i,
			"Resources/ui/icon_sword_1.png", "Resources/ui/icon_sword_1.png", "Resources/ui/icon_sword_1.png", this, btnName);

		// �� �ڷᱸ���� ��ư �־��
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

	// ������ ǥ�ø� ���� �̹��� �� �ؽ�Ʈ �߰�
	uiDialog->AddImage(eInvenUI_CopperCoinImg, "Resources/ui/ui-coppericon.png");
	uiDialog->AddImage(eInvenUI_SilverCoinImg, "Resources/ui/ui-silvericon.png");
	uiDialog->AddImage(eInvenUI_GoldCoinImg, "Resources/ui/ui-goldicon.png");

	uiDialog->AddText(eInvenUI_CopperCoinText, Assets::GetFont(Assets::FontType_Coin), "00");
	uiDialog->AddText(eInvenUI_SilverCoinText, Assets::GetFont(Assets::FontType_Coin), "00");
	uiDialog->AddText(eInvenUI_GoldCoinText, Assets::GetFont(Assets::FontType_Coin), "000000");

	// ������ ���� �̹��� �ؽ�Ʈ ����
	uiDialog->GetImage(eInvenUI_CopperCoinImg)->SetPosition(Vector3(226, 229, 0));
	uiDialog->GetText(eInvenUI_CopperCoinText)->SetPosition(Vector3(192, 229, 0));
	uiDialog->GetText(eInvenUI_CopperCoinText)->SetDrawFormat(DT_RIGHT | DT_VCENTER);

	uiDialog->GetImage(eInvenUI_SilverCoinImg)->SetPosition(Vector3(188, 229, 0));
	uiDialog->GetText(eInvenUI_SilverCoinText)->SetPosition(Vector3(154, 229, 0));
	uiDialog->GetText(eInvenUI_SilverCoinText)->SetDrawFormat(DT_RIGHT | DT_VCENTER);

	uiDialog->GetImage(eInvenUI_GoldCoinImg)->SetPosition(Vector3(150, 229, 0));
	uiDialog->GetText(eInvenUI_GoldCoinText)->SetPosition(Vector3(52, 229, 0));
	uiDialog->GetText(eInvenUI_GoldCoinText)->SetDrawFormat(DT_RIGHT | DT_VCENTER);



	// Mouse�� ���� �ִ� Item ǥ�ø� ���� �̹��� �� �ؽ�Ʈ �߰�(���� �ٸ������� �̵� �ʿ�)
	uiDialog->AddImage(eInvenUI_PickedItemImg, "None");
	uiDialog->GetImage(eInvenUI_PickedItemImg)->SetScale(Vector3(0.58f, 0.58f, 0.0f));

	uiDialog->AddText(eInvenUI_PickedItemNumText, Assets::GetFont(Assets::FontType_NORMAL), "00");
	uiDialog->GetText(eInvenUI_PickedItemNumText)->SetScale(Vector3(0.7f, 0.7f, 0.0f));
	uiDialog->GetText(eInvenUI_PickedItemNumText)->SetDrawFormat(DT_RIGHT | DT_VCENTER);
	uiDialog->GetText(eInvenUI_PickedItemNumText)->SetText("");


	// �׽�Ʈ �ڵ�(�׽�Ʈ �� ����)
	// ������ ����
	/*for (int i = 0; i < m_InvenSize; ++i)
	{
	m_vecItem[i].first = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X", "Resources/ui/icon_shoulder_1.png");
	m_vecItem[i].second = 20;
	}*/
	// ������ ����
	//m_Money = 10101;
	//******************************************************************************************************************************************

	UpdateIcons();
}

void ComUIInventory::Update()
{
	/*uiDialog->GetImage(eInvenUI_PickedItemImg)->SetPosition(Vector3(mousePos.x - 16 - gameObject->transform->GetPosition().x, mousePos.y - 16 - gameObject->transform->GetPosition().y, 0));
	uiDialog->GetText(eInvenUI_PickedItemNumText)->SetPosition(Vector3(mousePos.x - 16 - gameObject->transform->GetPosition().x, mousePos.y + 4 - gameObject->transform->GetPosition().y, 0));
*/

// ���õ� �������� ���� �� ��ġ �̵�
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
	// �κ��丮 �ݱ�
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
	//	// �κ��丮 ������ �����ܳ��� ��ġ ����
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

	// ������

	// �̵����� �������� ������ ó��
	if (m_pBtnPicked)
	{
		// �̵����� �������� ���� ������ �ִ°�? üũ

		for (auto & btn : m_mapButtons)
		{
		}

		return;
	}

	UIBtnInvenIcon* pBtn = m_mapButtons[pSender->GetButtonName()];

	m_pComUIItemInfo->SetPosition(Vector3(gameObject->transform->GetPosition().x + 300.0f, gameObject->transform->GetPosition().y, 0.0f));
	m_pComUIItemInfo->SetIsVisible(true);
	m_pComUIItemInfo->SetItemInfo(pBtn->pItem);


	// ������ ���� �κ�
	/*UIBtnInvenIcon* pBtn = m_mapButtons[pSender->GetButtonName()];
	if (pBtn->pItem == NULL)
		return;

	// STEP2. TODO : �κ��丮���� �ش� �������� Ŭ������ �� �Ǹ�UI�� ���

	// STEP3. Ȯ�� ��ư�� Ŭ������ �� �ش� ĳ���Ϳ� ��� ����, ��� ��ư ������ ���� �ƹ� ���� ����
	// 3-1. ĳ���� Ÿ������ ĳ���͸� ã�´�. (ComChrEquipment)
	switch (pBtn->pItem->ChrType)
	{
	case eChrType_Undead:
		ComChrEquipment* pChrEquipment = (ComChrEquipment*)GameObject::Find("undead_01")->GetComponent("ComChrEquipment");
		pChrEquipment->Equip(pBtn->pItem);

		// �ش� ��ư�� ������ ����
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

	//// �ش� �������� ������
	//if (false == FindItem(Item->UID, ItemIndex))
	//{
	//	ItemIndex = 0;
	//	// �󽽷��� ã�Ƽ� ������
	//	if (false == FindEmptySlot(ItemIndex))
	//	{
	//		// UI Message : �κ��丮�� �� á���ϴ�.
	//		return make_pair(Item, ItemNum);
	//	}
	//	// �󽽷��� ������
	//	else
	//	{
	//		m_vecItem[ItemIndex] = make_pair(Item, ItemNum);
	//		// UI ������Ʈ
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
		// UI Message : �κ��丮�� �� á���ϴ�.
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
	//// Inventory Slot �̹��� �� �ؽ�Ʈ ������Ʈ
	//for (int i = 0; i < m_vecItem.size(); i++)
	//{
	//	// �ؽ��� ���� �κ�
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

	// Mouse�� ���� �ִ� Item �̹��� �� �ؽ�Ʈ ������Ʈ
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

	// ������ �̹��� �� �ؽ�Ʈ ������Ʈ
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