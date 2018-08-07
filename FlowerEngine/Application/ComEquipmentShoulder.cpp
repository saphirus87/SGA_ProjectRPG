#include "stdafx.h"
#include "ComEquipmentShoulder.h"
#include "ItemInfo.h"

ComEquipmentShoulder::ComEquipmentShoulder(CString szName) :
	Component(szName),
	pDataSholder(NULL)
{
}


ComEquipmentShoulder::~ComEquipmentShoulder()
{
	SAFE_DELETE(pDataSholder);
}

void ComEquipmentShoulder::Awake()
{
	pDataSholder = new EquipmentShoulder();
}

void ComEquipmentShoulder::Update()
{
}

void ComEquipmentShoulder::Render()
{
}
