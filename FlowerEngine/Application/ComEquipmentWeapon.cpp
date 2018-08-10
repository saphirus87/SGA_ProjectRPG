#include "stdafx.h"
#include "ComEquipmentWeapon.h"


ComEquipmentWeapon::ComEquipmentWeapon(CString szName) :
	Component(szName),
	m_pAnimation(NULL),
	m_pGOWeaponRight(NULL),
	m_pGOWeaponLeft(NULL),
	m_pGOShieldLeft(NULL),
	m_pRenderRight(NULL),
	m_pRenderLeft(NULL),
	m_pRenderShieldLeft(NULL)
{
	
}

ComEquipmentWeapon::~ComEquipmentWeapon()
{
}

void ComEquipmentWeapon::Awake()
{
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");

	// 오브젝트들 생성
	FactoryGameObject factory;
	
	// .X File Export시 Frame이 Max축으로 되어있음 [z, x, y축]
	m_vOffsetPosR = Vector3(0, 0, -6); // 보정위치 y축 아래로 조금 내림
	m_vOffsetPosL = Vector3(0, 0, -6);
	m_vOffsetPosLShield = Vector3(0, -5, 0); // 보정위치 팔 밖쪽으로 조금

	m_pGOWeaponRight = factory.CreateEquipment("Equipment_weapon", "Resources/character/Equipment/", "Sword_01.X", m_vOffsetPosR);	
	//m_pGOWeaponLeft = factory.CreateEquipment("Equipment_weapon", "Resources/character/Equipment/", "Sword_01.X", m_vOffsetPosL);
	m_pGOShieldLeft = factory.CreateEquipment("Equipment_Shield", "Resources/character/Equipment/", "Shield_01.X", m_vOffsetPosLShield);

	// 무기 칼날 아래 방향으로 돌려 잡음
	m_pGOWeaponRight->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
	//m_pGOWeaponLeft->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
	// 쉴드가 누워있어서 돌림
	m_pGOShieldLeft->transform->SetRotation(Vector3(D3DXToRadian(90), D3DXToRadian(-90), 0));

	// 어깨 렌더링 컴포넌트 미리 찾아둠
	m_pRenderRight = (ComRenderXMesh*)m_pGOWeaponRight->GetComponent("ComRenderXMesh");

	if (m_pGOWeaponLeft)
		m_pRenderLeft = (ComRenderXMesh*)m_pGOWeaponLeft->GetComponent("ComRenderXMesh");

	if (m_pGOShieldLeft)
		m_pRenderShieldLeft = (ComRenderXMesh*)m_pGOShieldLeft->GetComponent("ComRenderXMesh");
}

void ComEquipmentWeapon::Update()
{
}

void ComEquipmentWeapon::Render()
{
	// Weapon_Right
	m_pRenderRight->SetFrameMatrix(m_pAnimation->GetMatrixByName("Weapon_Right"), &gameObject->transform->GetWorldMatrix());

	if (m_pRenderLeft)
		m_pRenderLeft->SetFrameMatrix(m_pAnimation->GetMatrixByName("Weapon_Left"), &gameObject->transform->GetWorldMatrix());

	if (m_pRenderShieldLeft)
		m_pRenderShieldLeft->SetFrameMatrix(m_pAnimation->GetMatrixByName("Shield_Left"), &gameObject->transform->GetWorldMatrix());
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