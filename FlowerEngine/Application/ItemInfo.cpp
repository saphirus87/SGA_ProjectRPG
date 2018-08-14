#include "stdafx.h"
#include "ItemInfo.h"


ItemInfo::ItemInfo() :
	ID(0),
	Type(eItem_None)
{
}

ItemInfo::~ItemInfo()
{
}

EquipmentShoulder::EquipmentShoulder() :
	HP(0),
	MP(0),
	DEF_PHY(0),
	DEF_MGR(0)
{
	Type = eItem_Shoulder;
	ChrType = eChrType_Human;
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

EquipmentHelmet::EquipmentHelmet() :
	HP(0),
	MP(0),
	DEF_PHY(0),
	DEF_MGR(0)
{
	Type = eItem_Helmet;
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

EquipmentShield::EquipmentShield() : 
	BLOCK_PER(10.0f)
{
	Type = eItem_Shield;
}

EquipmentShield::~EquipmentShield()
{
}

EquipmentWeapon::EquipmentWeapon() : 
	ATK_MIN(3),
	ATK_MAX(7)
{
	Type = eItem_WeaponR;
}

EquipmentWeapon::~EquipmentWeapon()
{
}
