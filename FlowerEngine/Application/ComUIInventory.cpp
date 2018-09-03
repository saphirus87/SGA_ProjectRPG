#include "stdafx.h"
#include "ComUIInventory.h"
#include "ItemInfo.h"

ComUIInventory::ComUIInventory(CString szName)
	: Component(szName), m_InvenSize(4)
{
}

ComUIInventory::~ComUIInventory()
{
}

void ComUIInventory::Awake()
{
	SetInvenSize(m_InvenSize);

	ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");

	for (int i = 0; i < m_InvenSize; ++i)
	{
		CString btnName;
		btnName.Format(L"InvenSlot%d", i + 1);
		uiDialog->AddButton(1001 + i, "None", "None", "None", this, btnName);
		uiDialog->GetButton(1001 + i)->SetScale(Vector3(0.58f, 0.58f, 0.0f));
		uiDialog->GetButton(1001 + i)->SetPosition(Vector3(81.5f + (float)(i % 4) * 41.5f, 51.5f + (float)(i / 4) * 41.0f, 0));

		uiDialog->AddText(1101 + i, Assets::GetFont(Assets::FontType_NORMAL), "00");
		uiDialog->GetText(1101 + i)->SetScale(Vector3(0.7f, 0.7f, 0.0f));
		uiDialog->GetText(1101 + i)->SetPosition(Vector3(81.5f + (float)(i % 4) * 41.5f, 71.5f + (float)(i / 4) * 41.0f, 0));
		uiDialog->GetText(1101 + i)->SetDrawFormat(DT_RIGHT | DT_VCENTER);
		uiDialog->GetText(1101 + i)->SetText("");
	}

	uiDialog->AddImage(2001, "None");
	uiDialog->GetImage(2001)->SetScale(Vector3(0.58f, 0.58f, 0.0f));

	uiDialog->AddText(2101, Assets::GetFont(Assets::FontType_NORMAL), "00");
	uiDialog->GetText(2101)->SetScale(Vector3(0.7f, 0.7f, 0.0f));
	uiDialog->GetText(2101)->SetDrawFormat(DT_RIGHT | DT_VCENTER);
	uiDialog->GetText(2101)->SetText("");

	for (int i = 0; i < m_InvenSize; ++i)
	{
		m_vecItem[i].first = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X", "Resources/ui/icon_shoulder_1.png");
		m_vecItem[i].second = 20;
	}

	UpdateIcons();
}

void ComUIInventory::Update()
{
	Vector2 mousePos = Input::GetMousePosition();
	ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");
	uiDialog->GetImage(2001)->SetPosition(Vector3(mousePos.x - 16 - gameObject->transform->GetPosition().x, mousePos.y - 16 - gameObject->transform->GetPosition().y, 0));
	uiDialog->GetText(2101)->SetPosition(Vector3(mousePos.x - 16 - gameObject->transform->GetPosition().x, mousePos.y + 4 - gameObject->transform->GetPosition().y, 0));
}

void ComUIInventory::Render()
{
}

void ComUIInventory::OnClick(UIButton* pSender)
{
	if (pSender->GetButtonName() == "InvenClose")
	{
		ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");
		uiDialog->SetIsVisible(false);
	}

	if (m_PickedItem.first == nullptr)
	{

	}
	else
	{
		CString slotName;
		for (int i = 0; i < m_InvenSize; i++)
		{
			slotName.Format(L"InvenSlot%d", i + 1);
			if (pSender->GetButtonName() == slotName)
			{
				m_PickedItem = InsertItemToSlot(m_PickedItem.first, m_PickedItem.second, i);

				UpdateIcons();
				break;
			}
		}
	}
}

void ComUIInventory::OnPress(UIButton * pSender)
{
	CString slotName;
	for (int i = 0; i < m_InvenSize; i++)
	{
		slotName.Format(L"InvenSlot%d", i + 1);
		if (pSender->GetButtonName() == slotName)
		{
			PickItem(i);
			UpdateIcons();
			break;
		}
	}
}

pair<ItemInfo*, UINT> ComUIInventory::AddItem(ItemInfo* Item, UINT ItemNum)
{
	int ItemIndex = 0;

	if (!FindItem(Item->ID, ItemIndex))
	{
		ItemIndex = 0;
		if (!FindEmptySlot(ItemIndex)) return make_pair(Item, ItemNum);
		else
		{
			m_vecItem[ItemIndex] = make_pair(Item, ItemNum);
			return make_pair(nullptr, 0);
		}
	}
	else
	{
		do
		{
			m_vecItem[ItemIndex].second += ItemNum;

			if (m_vecItem[ItemIndex].second > ItemMaxNum)
			{
				ItemNum = m_vecItem[ItemIndex].second - ItemMaxNum;
				m_vecItem[ItemIndex].second = ItemMaxNum;
			}
			else
			{
				ItemNum = 0;
				return make_pair(nullptr, 0);
			}
		} while (FindItem(Item->ID, ItemIndex) && ItemNum > 0);

		ItemIndex = 0;
		if (!FindEmptySlot(ItemIndex)) return make_pair(Item, ItemNum);
		else
		{
			m_vecItem[ItemIndex] = make_pair(Item, ItemNum);
			return make_pair(nullptr, 0);
		}
	}

	return make_pair(Item, ItemNum);
}

pair<ItemInfo*, UINT> ComUIInventory::InsertItemToSlot(ItemInfo * Item, UINT ItemNum, int InvenSlot)
{
	if (m_vecItem[InvenSlot].first == nullptr)
	{
		m_vecItem[InvenSlot] = make_pair(Item, ItemNum);

		return make_pair(nullptr, 0);
	}

	if (Item->ID == m_vecItem[InvenSlot].first->ID && m_vecItem[InvenSlot].second < ItemMaxNum)
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
	}
}

pair<ItemInfo*, UINT> ComUIInventory::DeleteItem(int InvenSlot, UINT ItemNum)
{
	if (InvenSlot < m_vecItem.size())
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

	return make_pair(nullptr, 0);
}

bool ComUIInventory::FindItem(unsigned int ItemID, int& StartIndex)
{
	for (; StartIndex < m_vecItem.size(); ++StartIndex)
	{
		if (m_vecItem[StartIndex].first->ID == ItemID) return true;
	}

	return false;
}

bool ComUIInventory::FindEmptySlot(int& StartIndex)
{
	for (; StartIndex < m_vecItem.size(); ++StartIndex)
	{
		if (m_vecItem[StartIndex].first == nullptr) return true;
	}

	return false;
}

bool ComUIInventory::PickItem(int InvenSlot)
{
	m_PickedItem = DeleteItem(InvenSlot, 0);

	return true;
}

void ComUIInventory::UpdateIcons()
{
	ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");

	for (int i = 0; i < m_vecItem.size(); i++)
	{
		if (m_vecItem[i].first == nullptr)
			uiDialog->GetButton(1001 + i)->SetTexture("None", "None", "None");
		else
			uiDialog->GetButton(1001 + i)->SetTexture(m_vecItem[i].first->IconName, m_vecItem[i].first->IconName, m_vecItem[i].first->IconName);

		if (m_vecItem[i].second <= 1)
			uiDialog->GetText(1101 + i)->SetText("");
		else
		{
			CString num;
			num.Format(L"%d", m_vecItem[i].second);
			uiDialog->GetText(1101 + i)->SetText(num);
		}
	}

	if (m_PickedItem.first == nullptr)
		uiDialog->GetImage(2001)->SetTexture("None");
	else
		uiDialog->GetImage(2001)->SetTexture(m_PickedItem.first->IconName);

	if (m_PickedItem.second <= 1)
		uiDialog->GetText(2101)->SetText("");
	else
	{
		CString num;
		num.Format(L"%d", m_PickedItem.second);
		uiDialog->GetText(2101)->SetText(num);
	}
}

bool ComUIInventory::SetInvenSize(UINT InvenSize)
{
	if (m_InvenSize <= InvenSize)
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
	}
}