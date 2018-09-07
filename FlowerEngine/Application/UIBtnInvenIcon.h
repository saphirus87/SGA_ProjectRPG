#pragma once
#include "stdafx.h"

class UIBtnInvenIcon : public UIButton
{
public:
	UIBtnInvenIcon(UIButtonDelegate* pDelegate, CString buttonName);
	~UIBtnInvenIcon();

	// 이 버튼이 표시할 아이템 정보 포인터
	ItemInfo* pItem;

	virtual void Render() override;
};

