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
	if (Input::KeyDown('1'))
		m_pAnimation->PlayAnimation(eAni_Stand);
	
	if (Input::KeyDown('2'))
		m_pAnimation->PlayAnimation(eAni_Walk);

	if (Input::KeyDown('3'))
		m_pAnimation->PlayAnimation(eAni_Attack_1);

	if (Input::KeyDown('4'))
		m_pAnimation->PlayAnimation(eAni_Attack_2);

	if (Input::KeyDown('5'))
		m_pAnimation->PlayAnimation(eAni_Attack_3);
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
