#pragma once
#include "stdafx.h"

class EquipmentShoulder;

enum eEquipment
{
	eEquipment_ShoulderR,
	eEquipment_ShoulderL,
	eEquipment_Helmet,
	eEquipment_WeaponR,
	eEquipment_WeaponL,
	eEquipment_Shield,
	eEquipment_Count
};

class RenderEquipment
{
public:
	RenderEquipment();
	~RenderEquipment();

	void Set(LPCSTR szName, GameObject* pGOParent, GameObject* pGOEquipment);

	void Update();

	LPCSTR szFrameName;
	// ��ġ ������
	Vector3 m_vOffsetPos;
	// �� ����� �θ� ���� ������Ʈ
	GameObject* m_pGOParent;
	// ��� ������Ʈ
	GameObject* m_pGOEquipment;
	// ������ �������
	ComRenderXMesh* m_pRender;
	// �ִϸ��̼� ������
	ComRenderSkinnedMesh * m_pAnimation;
};

class ComEquipment : public Component
{
public:
	ComEquipment(CString szName);
	~ComEquipment();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// ����� �������� Export���� �����Ƿ� �������� �����Ͽ� ��ġ�� ������ �ݴϴ�.
	// .X File Export�� Frame�� Max������ �Ǿ����� [z, x, y��]
	void SetOffsetPos(Vector3 vOffsetPosR = Vector3(3, 10, -8));

	// ������ �̸����� �ؽ��ĸ� �����մϴ�.
	void ChangeTexture(CString szItemName);

	EquipmentShoulder* pDataSholder;

private:
	map<CString, CString> m_mapTextureName;

	// ĳ���� �ִϸ��̼� �������
	ComRenderSkinnedMesh * m_pAnimation;

	vector<RenderEquipment*> m_vecRenderEquipments;

	// ��ġ ������ ������, ����
	Vector3 m_vOffsetPosR;
	Vector3 m_vOffsetPosL;

	// ��� �� ���ӿ�����Ʈ ������, ����
	GameObject* m_pGOShoulderRight;
	GameObject* m_pGOShoulderLeft;
	GameObject* m_pGOHelmet;

	// ��� �� ������ ������� ������, ����
	ComRenderXMesh* m_pRenderRight;
	ComRenderXMesh* m_pRenderLeft;
	ComRenderXMesh* m_pRenderHelmet;
};

