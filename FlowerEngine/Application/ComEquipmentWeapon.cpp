#include "stdafx.h"
#include "ComEquipmentWeapon.h"


ComEquipmentWeapon::ComEquipmentWeapon(CString szName) :
	Component(szName),
	m_pGOWeaponRight(NULL),
	m_pGOWeaponLeft(NULL),
	m_pAnimation(NULL)
{
	
}


ComEquipmentWeapon::~ComEquipmentWeapon()
{
}

void ComEquipmentWeapon::Awake()
{
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");

	// ������Ʈ�� ����
	FactoryGameObject factory;
	
	// .X File Export�� Frame�� Max������ �Ǿ����� [z, x, y��]
	m_vOffsetPosR = Vector3(0, 0, -6); // ������ġ y�� �Ʒ��� ���� ����
	m_vOffsetPosL = Vector3(0, 0, -6);

	m_pGOWeaponRight = factory.CreateEquipmentShoulder("Equipment_weapon", "Resources/character/Equipment/", "Sword_01.X", m_vOffsetPosR);	

	// ���� Į�� �Ʒ� �������� ���� ����
	m_pGOWeaponRight->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
	m_pGOWeaponLeft = factory.CreateEquipmentShoulder("Equipment_weapon", "Resources/character/Equipment/", "Sword_01.X", m_vOffsetPosL);
	m_pGOWeaponLeft->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));

	// ��� ������ ������Ʈ �̸� ã�Ƶ�
	m_pRenderRight = (ComRenderXMesh*)m_pGOWeaponRight->GetComponent("ComRenderXMesh");

	if (m_pGOWeaponLeft)
		m_pRenderLeft = (ComRenderXMesh*)m_pGOWeaponLeft->GetComponent("ComRenderXMesh");
}

void ComEquipmentWeapon::Update()
{
}

void ComEquipmentWeapon::Render()
{
	m_pRenderRight->SetFrameMatrix(&m_pAnimation->GetMatrixByName("character_human_male_humanmale_hd_bone_46"), &gameObject->transform->GetWorldMatrix());

	if (m_pRenderLeft)
		m_pRenderLeft->SetFrameMatrix(&m_pAnimation->GetMatrixByName("character_human_male_humanmale_hd_bone_41"), &gameObject->transform->GetWorldMatrix());
}

void ComEquipmentWeapon::SetOffsetPos(Vector3 vOffsetPosR)
{
	m_vOffsetPosR = vOffsetPosR;
	m_vOffsetPosL = m_vOffsetPosR;
	m_vOffsetPosL.y *= -1;

	if (m_pGOWeaponRight)
		m_pGOWeaponRight->transform->SetPosition(m_vOffsetPosR);
	if (m_pGOWeaponLeft)
		m_pGOWeaponLeft->transform->SetPosition(m_vOffsetPosL);
}