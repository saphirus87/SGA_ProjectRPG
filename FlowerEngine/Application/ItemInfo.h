/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

���� :
Ŭ���� �̸� �׵��� ������ �����Դϴ�. ���� EXCEL�̳� ITEM TOOL���� �����͸� �о ����մϴ�.
*/
#pragma once
#include "stdafx.h"

enum eItemType
{
	eItem_None,		// Ÿ�� ����
	eItem_Shoulder,	// ��� ��
	eItem_COUNT
};

// ������ ����
class ItemInfo
{
public:
	ItemInfo();
	virtual ~ItemInfo();

	// ������ ���̵� 
	unsigned int ID;
	eItemType ItemType;
};

// ��� ������
class EquipmentShoulder : public ItemInfo
{
public:
	EquipmentShoulder();
	~EquipmentShoulder();

	// ĳ������ �ɷ�ġ�� ü��(Hit Point) ����
	int HP;
	// ĳ������ �ɷ�ġ�� ����(Mana Point) ����
	int MP;
	// ĳ������ �ɷ�ġ�� ���� ���� ����
	int DEF_PHY;
	// ĳ������ �ɷ�ġ�� ���� ���� ����
	int DEF_MGR;
};