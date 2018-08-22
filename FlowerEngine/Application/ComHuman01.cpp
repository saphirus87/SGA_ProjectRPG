#include "stdafx.h"
#include "ComHuman01.h"
#include "ComChrEquipment.h"
#include "ComEquipment.h"
#include "ItemInfo.h"
#include "ComCharacter.h"
#include "ComChrControl.h"

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

HRESULT HumanAttackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	CString szDebug;
	szDebug.Format(L"EventCallback Track : %d\r\n", Track);
	OutputDebugString(szDebug);

	// 특정 프레임에서 공격
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");
	pChr->AttackTarget(pControl->pAttackTarget);

	return S_OK;
}