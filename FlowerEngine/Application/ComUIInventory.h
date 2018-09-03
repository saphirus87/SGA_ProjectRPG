#pragma once
#include "stdafx.h"

const UINT ItemMaxNum = 99;

class ComUIInventory : public Component, public UIButtonDelegate
{
public:
	ComUIInventory(CString szName);
	~ComUIInventory();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// UIButtonDelegate을(를) 통해 상속됨
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;

	//
	pair<ItemInfo*, UINT> AddItem(ItemInfo* Item, UINT ItemNum);
	pair<ItemInfo*, UINT> InsertItemToSlot(ItemInfo* Item, UINT ItemNum, int InvenSlot);
	//bool AddItem(ItemInfo*);

	pair<ItemInfo*, UINT> DeleteItem(int InvenSlot, UINT ItemNum);
	//pair<CString, UINT> DeleteItem(ItemInfo* pItem, int Num); // 어떤 아이템, 개수 몇개를 지운다
	bool FindItem(unsigned int ItemID, int& StartIndex);
	bool FindEmptySlot(int& StartIndex);
	bool PickItem(int InvenSlot);
	void UpdateIcons();

	bool SetInvenSize(UINT InvenSize);

private:
	vector<pair<ItemInfo*, UINT>> m_vecItem;

	//vector<pair<ItemInfo*, UINT>> m_vecUIItem;

	// list<ItemInfo*> m_listItems;
	// for (auto & item : m_listItems) item->ID, item->Count
	// (다형성) ((EquipShoulder*)pItemInfo)->ATK_MIN
	UINT m_InvenSize;

	pair<ItemInfo*, UINT> m_PickedItem;
};

