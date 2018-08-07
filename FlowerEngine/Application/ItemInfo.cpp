#include "stdafx.h"
#include "ItemInfo.h"


ItemInfo::ItemInfo() :
	ID(0),
	ItemType(eItem_None)

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
}

EquipmentShoulder::~EquipmentShoulder()
{
}
