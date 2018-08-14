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
	eItem_None = -1,
	eItem_Shoulder,
	eItem_Helmet,
	eItem_Shield,
	eItem_WeaponR,
	eItem_WeaponL,
	// �߰�
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
	// ���� ĳ���� Ÿ��
	eChrType ChrType;
	// ������ �̸�
	CString Name;
	// ���� ���
	CString FolderPath;
	// ������ ���ҽ� �̸�
	CString XFileName;
	// ����� �ؽ��� �̸�, �ؽ��� �̸��� ������ ���� ���
	CString TextureName;
};

// ��� �� ��� ������
class EquipmentShoulder : public ItemInfo
{
public:
	EquipmentShoulder(CString szName, CString szXFileName);
	~EquipmentShoulder();

	void Set(int iHP, int iMP, int iDEF_PHY, int iDEF_MGR, eChrType chrType);

	// TODO : �ɷ�ġ ���� ��ȹ�� �°� ���� ����
	// ĳ������ �ɷ�ġ�� ü��(Hit Point) ����
	int HP;
	// ĳ������ �ɷ�ġ�� ����(Mana Point) ����
	int MP;
	// ĳ������ �ɷ�ġ�� ���� ���� ����
	int DEF_PHY;
	// ĳ������ �ɷ�ġ�� ���� ���� ����
	int DEF_MGR;
	// �ɷ�ġ �߰�
};

// �۸� �� ��� ������
class EquipmentHelmet : public ItemInfo
{
public:
	EquipmentHelmet(CString szName, CString szXFileName);
	~EquipmentHelmet();

	void Set(int iHP, int iMP, int iDEF_PHY, int iDEF_MGR, eChrType chrType);

	// TODO : �ɷ�ġ ���� ��ȹ�� �°� ���� ����
	// ĳ������ �ɷ�ġ�� ü��(Hit Point) ����
	int HP;
	// ĳ������ �ɷ�ġ�� ����(Mana Point) ����
	int MP;
	// ĳ������ �ɷ�ġ�� ���� ���� ����
	int DEF_PHY;
	// ĳ������ �ɷ�ġ�� ���� ���� ����
	int DEF_MGR;
	// �ɷ�ġ �߰�
};

// ���� �� ��� ������
class EquipmentShield : public ItemInfo
{
public:
	EquipmentShield(CString szName, CString szXFileName);
	~EquipmentShield();

	void Set(int iBLOCK_PER, eChrType chrType);

	// TODO : �ɷ�ġ ���� ��ȹ�� �°� ���� ����
	// ĳ������ �ɷ�ġ�� ���� ���� Ȯ�� ����
	float BLOCK_PER;
};

// �� ���� ������
class EquipmentWeapon : public ItemInfo
{
public:
	EquipmentWeapon(CString szName, CString szXFileName);
	~EquipmentWeapon();

	void Set(int iATK_MIN, int iATK_MAX, eChrType chrType);

	// TODO : �ɷ�ġ ���� ��ȹ�� �°� ���� ����
	// ĳ������ �ɷ�ġ�� �ּ� ���ݷ� ����
	int ATK_MIN;
	// ĳ������ �ɷ�ġ�� �ִ� ���ݷ� ����
	int ATK_MAX;
};

// ��� Ŭ���� �߰�
//class EquipmentStaff : public ItemInfo