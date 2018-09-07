#include "stdafx.h"
#include "UIBtnInvenIcon.h"
#include "ItemInfo.h"

UIBtnInvenIcon::UIBtnInvenIcon(UIButtonDelegate* pDelegate, CString buttonName) :
	UIButton(pDelegate, buttonName),
	pItem(NULL)
{
}


UIBtnInvenIcon::~UIBtnInvenIcon()
{
}

void UIBtnInvenIcon::Render()
{
	if (pItem)
	{
		SetTexture(pItem->IconName, pItem->IconName, pItem->IconName);

		UIButton::Render();
	}
}
