#include "stdafx.h"
#include "ComInventory.h"

ComInventory::ComInventory(CString szName)
	: Component(szName), m_InvenSize(4)
{
}

ComInventory::~ComInventory()
{
}

void ComInventory::Awake()
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
	}

	uiDialog->AddImage(2001, "None");
	uiDialog->GetImage(2001)->SetScale(Vector3(0.58f, 0.58f, 0.0f));

	for (int i = 0; i < m_InvenSize; ++i)
	{
		m_vecItem[i].first = "giftbox01";
		m_vecItem[i].second = 1;
	}

	UpdateIcons();
}

void ComInventory::Update()
{
	Vector2 mousePos = Input::GetMousePosition();
	ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");
	uiDialog->GetImage(2001)->SetPosition(Vector3(mousePos.x - 16 - gameObject->transform->GetPosition().x, mousePos.y - 16 - gameObject->transform->GetPosition().y, 0));
}

void ComInventory::Render()
{
}

void ComInventory::OnClick(UIButton* pSender)
{
	if (pSender->GetButtonName() == "InvenClose")
	{
		ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");
		uiDialog->SetIsVisible(false);
	}

	if (m_PickedItem.first == "NoItem")
	{

	}
	else
	{
		pair<CString, UINT> temp = m_PickedItem;

		CString slotName;
		for (int i = 0; i < m_InvenSize; i++)
		{
			slotName.Format(L"InvenSlot%d", i + 1);
			if (pSender->GetButtonName() == slotName)
			{
				PickItem(i);
				m_vecItem[i] = temp;
				UpdateIcons();
				break;
			}
		}
	}
}

void ComInventory::OnPress(UIButton * pSender)
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

bool ComInventory::AddItem(CString szItemName, UINT ItemNum)
{
	int ItemIndex = 0;

	if (!FindItem(szItemName, ItemIndex))
	{
		ItemIndex = 0;
		if (!FindEmptySlot(ItemIndex)) return false;
		else
		{
			m_vecItem[ItemIndex] = make_pair(szItemName, ItemNum);
			return true;
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
			}
		} while (FindItem(szItemName, ItemIndex) && ItemNum > 0);

		ItemIndex = 0;
		if (!FindEmptySlot(ItemIndex)) return false;
		else
		{
			m_vecItem[ItemIndex] = make_pair(szItemName, ItemNum);
			return true;
		}
	}

	return false;
}

pair<CString, UINT> ComInventory::DeleteItem(int InvenNum)
{
	if (InvenNum < m_vecItem.size())
	{
		pair<CString, UINT> DeletedItem(m_vecItem[InvenNum]);
		m_vecItem[InvenNum] = make_pair("NoItem", 0);

		return DeletedItem;
	}

	return make_pair("NoItem", 0);
}

bool ComInventory::FindItem(CString szItemName, int& StartIndex)
{
	for (; StartIndex < m_vecItem.size(); ++StartIndex)
	{
		if (m_vecItem[StartIndex].first == szItemName) return true;
	}

	return false;
}

bool ComInventory::FindEmptySlot(int& StartIndex)
{
	for (; StartIndex < m_vecItem.size(); ++StartIndex)
	{
		if (m_vecItem[StartIndex].first == "NoItem") return true;
	}

	return false;
}

bool ComInventory::PickItem(int InvenNum)
{
	m_PickedItem = DeleteItem(InvenNum);

	return true;
}

void ComInventory::UpdateIcons()
{
	ComDialog* uiDialog = (ComDialog*)gameObject->GetComponent("ComDialog");

	for (int i = 0; i < m_vecItem.size(); i++)
	{
		if (m_vecItem[i].first == "NoItem")
			uiDialog->GetButton(1001 + i)->SetTexture("None", "None", "None");
		else if (m_vecItem[i].first == "giftbox01")
			uiDialog->GetButton(1001 + i)->SetTexture("Resources/ui/giftbox01.png", "Resources/ui/giftbox01_over.png", "Resources/ui/giftbox01.png");
	}

	if (m_PickedItem.first == "NoItem")
		uiDialog->GetImage(2001)->SetTexture("None");
	else if (m_PickedItem.first == "giftbox01")
		uiDialog->GetImage(2001)->SetTexture("Resources/ui/giftbox01.png");
}

bool ComInventory::SetInvenSize(UINT InvenSize)
{
	if (m_InvenSize <= InvenSize)
	{
		m_vecItem.resize(InvenSize, make_pair("NoItem", 0));
		m_InvenSize = InvenSize;

		return true;
	}
	else
	{
		for (int i = InvenSize; i < m_InvenSize; ++i)
		{
			if (m_vecItem[i].first != "NoItem") return false;
		}

		m_vecItem.resize(InvenSize);
		m_InvenSize = InvenSize;

		return true;
	}
}