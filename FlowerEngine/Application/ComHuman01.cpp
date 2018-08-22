#include "stdafx.h"
#include "ComHuman01.h"
#include "ComChrEquipment.h"
#include "ComEquipment.h"
#include "ItemInfo.h"
#include "ComCharacter.h"

ComHuman01::ComHuman01(CString szName) : 
	ComCharacter(szName)
{
}

ComHuman01::~ComHuman01()
{
}

void ComHuman01::Awake()
{
	Init();
}

void ComHuman01::Update()
{
	
}

void ComHuman01::Render()
{
}

void ComHuman01::OnTriggerEnter(ComCollider & collider)
{
	if (collider.gameObject->Name().Find(L"Equipment_shoulder") >= 0)
	{
		ComChrEquipment* pEquipment = (ComChrEquipment*)gameObject->GetComponent("ComChrEquipment");
		if (pEquipment != NULL)
		{
			ComEquipment* pEquip = (ComEquipment*)collider.gameObject->GetComponent("ComEquipment");
			pEquipment->Equip(pEquip->pItemInfo);
			collider.gameObject->SetActive(false);
		}
	}
}
