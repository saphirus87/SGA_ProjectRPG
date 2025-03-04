#include "stdafx.h"
#include "ItemInfo.h"


ItemInfo::ItemInfo() :
	UID(0),
	Type(eItem_None),
	Reference(0)
{
	FolderPath = "Resources/character/Equipment/";
}

ItemInfo::~ItemInfo()
{
}

ItemPotion::ItemPotion() : 
	Count(1), CountMax(100)
{
}

ItemPotion::~ItemPotion()
{
}


EquipmentShoulder::EquipmentShoulder(CString szName, CString szXFileName, CString szIconName) :
	HP(0),
	MP(0),
	DEF_PHY(0),
	DEF_MGR(0)
{
	Type = eItem_Shoulder;
	ChrType = eChrType_Human;
	Name = szName;
	XFileName = szXFileName;
	IconName = szIconName;
}

EquipmentShoulder::~EquipmentShoulder()
{
}

void EquipmentShoulder::Set(int iHP, int iMP, int iDEF_PHY, int iDEF_MGR, eChrType chrType)
{
	Type = eItem_Shoulder;
	ChrType = chrType;
	HP = iHP;
	MP = iMP;
	DEF_PHY = iDEF_PHY;
	DEF_MGR = iDEF_MGR;
}

EquipmentHelmet::EquipmentHelmet(CString szName, CString szXFileName, CString szIconName) :
	HP(0),
	MP(0),
	DEF_PHY(0),
	DEF_MGR(0)
{
	Type = eItem_Helmet;
	ChrType = eChrType_Human;
	Name = szName;
	XFileName = szXFileName;
	IconName = szIconName;
}

EquipmentHelmet::~EquipmentHelmet()
{
}

void EquipmentHelmet::Set(int iHP, int iMP, int iDEF_PHY, int iDEF_MGR, eChrType chrType)
{
	Type = eItem_Helmet;
	ChrType = chrType;
	HP = iHP;
	MP = iMP;
	DEF_PHY = iDEF_PHY;
	DEF_MGR = iDEF_MGR;
}

EquipmentShield::EquipmentShield(CString szName, CString szXFileName, CString szIconName) :
	BLOCK_PER(10.0f)
{
	Type = eItem_Shield;
	ChrType = eChrType_Human;
	Name = szName;
	XFileName = szXFileName;
	IconName = szIconName;
}

EquipmentShield::~EquipmentShield()
{
}

void EquipmentShield::Set(int iBLOCK_PER, eChrType chrType)
{
	Type = eItem_Shield;
	ChrType = chrType;
	BLOCK_PER = iBLOCK_PER;
}

EquipmentWeapon::EquipmentWeapon(CString szName, CString szXFileName, CString szIconName) :
	ATK_MIN(3),
	ATK_MAX(7)
{
	Type = eItem_WeaponR;
	ChrType = eChrType_Human;
	Name = szName;
	XFileName = szXFileName;
	IconName = szIconName;
}

EquipmentWeapon::~EquipmentWeapon()
{
}

void EquipmentWeapon::Set(int iATK_MIN, int iATK_MAX, eChrType chrType)
{
	Type = eItem_WeaponR;
	ChrType = chrType;
	ATK_MIN = iATK_MIN;
	ATK_MAX = iATK_MAX;
}
