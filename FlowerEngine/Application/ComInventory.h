#pragma once
#include "stdafx.h"

const UINT ItemMaxNum = 99;

class ComInventory : public Component, public UIButtonDelegate
{
public:
	ComInventory(CString szName);
	~ComInventory();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
	
	// UIButtonDelegate을(를) 통해 상속됨
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;

	//
	bool AddItem(CString szItemName, UINT ItemNum);
	pair<CString, UINT> DeleteItem(int InvenNum);
	bool FindItem(CString szItemName, int& StartIndex);
	bool FindEmptySlot(int& StartIndex);

	bool SetInvenSize(UINT InvenSize);

private:
	vector<pair<CString, UINT>> m_vecItem;
	UINT m_InvenSize;
};

