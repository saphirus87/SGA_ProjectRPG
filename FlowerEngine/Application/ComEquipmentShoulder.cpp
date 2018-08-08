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
	
	m_mapTextureName.insert(map<CString, CString>::value_type("ShoulderEquipItemName01", "Resources/character/Equipment/shoulder_plate_d_02copper.png"));
	m_mapTextureName.insert(map<CString, CString>::value_type("ShoulderEquipItemName02", "Resources/character/Equipment/shoulder_robe_b_03blue.png"));

	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");

	SetOffsetPos();

	// 오브젝트들 생성
	FactoryGameObject factory;
	m_pGOShoulderRight = factory.CreateEquipmentShoulder("Equipment_shoulder", "Resources/character/Equipment/", "shoulder_01.X", m_vOffsetPosR);	// .X File Export시 Frame이 Max축으로 되어있음 [z, x, y축]
	m_pGOShoulderLeft = factory.CreateEquipmentShoulder("Equipment_shoulder", "Resources/character/Equipment/", "shoulder_01.X", m_vOffsetPosL, true);	// .X File Export시 Frame이 Max축으로 되어있음 [z, x, y축]

	// 어깨 렌더링 컴포넌트 미리 찾아둠
	m_pRenderRight = (ComRenderXMesh*)m_pGOShoulderRight->GetComponent("ComRenderXMesh");
	m_pRenderLeft = (ComRenderXMesh*)m_pGOShoulderLeft->GetComponent("ComRenderXMesh");

	// shoulder_robe_b_03blue.png
}

void ComEquipmentShoulder::Update()
{
}

void ComEquipmentShoulder::Render()
{
	m_pRenderRight->SetFrameMatrix(&m_pAnimation->GetMatrixByName("Shoulder_Right"), &gameObject->transform->GetWorldMatrix());
	m_pRenderLeft->SetFrameMatrix(&m_pAnimation->GetMatrixByName("Shoulder_Left"), &gameObject->transform->GetWorldMatrix());
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

void ComEquipmentShoulder::ChangeTexture(CString szItemName)
{
	m_pRenderRight->ChangeTexture(0, m_mapTextureName[szItemName]);
	m_pRenderLeft->ChangeTexture(0, m_mapTextureName[szItemName]);
}
