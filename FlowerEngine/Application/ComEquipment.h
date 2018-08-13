#pragma once
#include "stdafx.h"

class EquipmentShoulder;
class ComRenderEquipment;
class ItemInfo;

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

	void Redner();

	// ���̸�
	LPCSTR szFrameName;
	// ��ġ ������
	Vector3 m_vOffsetPos;
	// �� ����� �θ� ���� ������Ʈ
	GameObject* m_pGOParent;
	// ��� ������Ʈ
	GameObject* m_pGOEquipment;
	// ������ �������
	ComRenderEquipment* m_pRender;
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
	void SetOffsetPos(eEquipment type, Vector3 vOffsetPos = Vector3(3, 10, -8));

	// ������ �̸����� �ؽ��ĸ� �����մϴ�.
	void ChangeTexture(eEquipment type, CString szItemName);

	// ��� �����մϴ�.
	void Equip(ItemInfo* pItem);
	
private:
	map<CString, CString> m_mapTextureName;
	
	// ������ ��� �����۵�
	vector<ItemInfo*> m_vecEquipedItems;

	// ������ �� ��� �����۵�
	vector<RenderEquipment*> m_vecRenderEquipments;
};

