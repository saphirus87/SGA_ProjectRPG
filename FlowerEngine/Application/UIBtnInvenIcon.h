#pragma once
#include "stdafx.h"

class UIBtnInvenIcon : public UIButton
{
public:
	UIBtnInvenIcon(UIButtonDelegate* pDelegate, CString buttonName);
	~UIBtnInvenIcon();

	// �� ��ư�� ǥ���� ������ ���� ������
	ItemInfo* pItem;

	virtual void Render() override;
};

