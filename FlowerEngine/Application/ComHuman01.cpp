#include "stdafx.h"
#include "ComHuman01.h"
#include "ComEquipment.h"
#include "ItemInfo.h"

ComHuman01::ComHuman01(CString szName) : 
	Component(szName),
	m_pAnimation(NULL)
{
}

ComHuman01::~ComHuman01()
{
}

void ComHuman01::Awake()
{
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
}

void ComHuman01::Update()
{
	if (Input::KeyDown('1'))
		m_pAnimation->PlayAnimation(eAni_Stand);
	
	if (Input::KeyDown('2'))
		m_pAnimation->PlayAnimation(eAni_Walk);
}

void ComHuman01::Render()
{
}

void ComHuman01::OnTriggerEnter(ComCollider & collider)
{
	if (collider.gameObject->Name().Find(L"Equipment_shoulder") >= 0)
	{
		ComEquipment* pEquipment = (ComEquipment*)gameObject->GetComponent("ComEquipment");
		if (pEquipment != NULL)
		{
			EquipmentShoulder* pShoulder = new EquipmentShoulder;
			pShoulder->Set(10, 10, 10, 10);
			pEquipment->Equip(pShoulder, collider.gameObject);
		}
	}
}
