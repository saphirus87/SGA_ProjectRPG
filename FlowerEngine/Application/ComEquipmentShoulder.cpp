#include "stdafx.h"
#include "ComEquipmentShoulder.h"
#include "ItemInfo.h"

ComEquipmentShoulder::ComEquipmentShoulder(CString szName) :
	Component(szName),
	pDataSholder(NULL),
	m_pRenderRight(NULL),
	m_pRenderLeft(NULL),
	m_pAnimation(NULL),
	m_pGOShoulderRight(NULL),
	m_pGOShoulderLeft(NULL)
{
}

ComEquipmentShoulder::~ComEquipmentShoulder()
{
	SAFE_DELETE(pDataSholder);
}

void ComEquipmentShoulder::Awake()
{
	pDataSholder = new EquipmentShoulder();

	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");

	SetOffsetPos();

	// ������Ʈ�� ����
	FactoryGameObject factory;
	m_pGOShoulderRight = factory.CreateEquipmentShoulder("Equipment_shoulder", "Resources/character/Equipment/", "shoulder_01.X", m_vOffsetPosR);	// .X File Export�� Frame�� Max������ �Ǿ����� [z, x, y��]
	m_pGOShoulderLeft = factory.CreateEquipmentShoulder("Equipment_shoulder", "Resources/character/Equipment/", "shoulder_01.X", m_vOffsetPosL, true);	// .X File Export�� Frame�� Max������ �Ǿ����� [z, x, y��]

	// ��� ������ ������Ʈ �̸� ã�Ƶ�
	m_pRenderRight = (ComRenderXMesh*)m_pGOShoulderRight->GetComponent("ComRenderXMesh");
	m_pRenderLeft = (ComRenderXMesh*)m_pGOShoulderLeft->GetComponent("ComRenderXMesh");
}

void ComEquipmentShoulder::Update()
{
	m_pRenderRight->SetFrameMatrix(&m_pAnimation->GetMatrixByName("Shoulder_Right"), &gameObject->transform->GetWorldMatrix());
	m_pRenderLeft->SetFrameMatrix(&m_pAnimation->GetMatrixByName("Shoulder_Left"), &gameObject->transform->GetWorldMatrix());
}

void ComEquipmentShoulder::Render()
{
}

void ComEquipmentShoulder::SetOffsetPos(Vector3 vOffsetPosR)
{
	m_vOffsetPosR = vOffsetPosR;
	m_vOffsetPosL = m_vOffsetPosR;
	m_vOffsetPosL.y *= -1;
	
	if (m_pGOShoulderRight)
		m_pGOShoulderRight->transform->SetPosition(m_vOffsetPosR);
	if (m_pGOShoulderLeft)
		m_pGOShoulderLeft->transform->SetPosition(m_vOffsetPosL);
}
