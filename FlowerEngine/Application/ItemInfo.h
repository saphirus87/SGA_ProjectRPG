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
	eItem_None,
	eItem_Shoulder,
	eItem_Helmet,
	eItem_Shield,
	eItem_WeaponR,
	eItem_WeaponL,
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
	// ������ Ÿ��
	eItemType Type;
	// ������ �̸�
	CString Name;
	// ���� ĳ���� Ÿ��
	eChrType ChrType;
};

// ��� �� ��� ������
class EquipmentShoulder : public ItemInfo
{
public:
	EquipmentShoulder();
	~EquipmentShoulder();

	void Set(int iHP, int iMP, int iDEF_PHY, int iDEF_MGR, eChrType chrType = eChrType_Human);

	// TODO : �ɷ�ġ ���� ��ȹ�� �°� ���� ����
	// ĳ������ �ɷ�ġ�� ü��(Hit Point) ����
	int HP;
	// ĳ������ �ɷ�ġ�� ����(Mana Point) ����
	int MP;
	// ĳ������ �ɷ�ġ�� ���� ���� ����
	int DEF_PHY;
	// ĳ������ �ɷ�ġ�� ���� ���� ����
	int DEF_MGR;
};

// �۸� �� ��� ������
class EquipmentHelmet : public ItemInfo
{
public:
	EquipmentHelmet();
	~EquipmentHelmet();

	void Set(int iHP, int iMP, int iDEF_PHY, int iDEF_MGR, eChrType chrType = eChrType_Human);

	// TODO : �ɷ�ġ ���� ��ȹ�� �°� ���� ����
	// ĳ������ �ɷ�ġ�� ü��(Hit Point) ����
	int HP;
	// ĳ������ �ɷ�ġ�� ����(Mana Point) ����
	int MP;
	// ĳ������ �ɷ�ġ�� ���� ���� ����
	int DEF_PHY;
	// ĳ������ �ɷ�ġ�� ���� ���� ����
	int DEF_MGR;
};

// ���� �� ��� ������
class EquipmentShield : public ItemInfo
{
public:
	EquipmentShield();
	~EquipmentShield();

	// TODO : �ɷ�ġ ���� ��ȹ�� �°� ���� ����
	// ĳ������ �ɷ�ġ�� ���� ���� Ȯ�� ����
	float BLOCK_PER;
};

// �� ���� ������
class EquipmentWeapon : public ItemInfo
{
public:
	EquipmentWeapon();
	~EquipmentWeapon();

	// TODO : �ɷ�ġ ���� ��ȹ�� �°� ���� ����
	// ĳ������ �ɷ�ġ�� �ּ� ���ݷ� ����
	int ATK_MIN;
	// ĳ������ �ɷ�ġ�� �ִ� ���ݷ� ����
	int ATK_MAX;
};