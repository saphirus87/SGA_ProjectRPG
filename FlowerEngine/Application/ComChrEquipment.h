#pragma once
#include "stdafx.h"

class EquipmentShoulder;
class ComEquipment;
class ItemInfo;

enum eRenderEquipment
{
	eRenderEquipment_ShoulderR,
	eRenderEquipment_ShoulderL,
	eRenderEquipment_Helmet,
	eRenderEquipment_Shield,
	eRenderEquipment_WeaponR,
	eRenderEquipment_WeaponL,
	eRenderEquipment_Count
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
	ComEquipment* m_pRender;
	// �ִϸ��̼� ������
	ComRenderSkinnedMesh * m_pAnimation;
};

class ComChrEquipment : public Component
{
public:
	ComChrEquipment(CString szName);
	~ComChrEquipment();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// ����� �������� Export���� �����Ƿ� �������� �����Ͽ� ��ġ�� ������ �ݴϴ�.
	// .X File Export�� Frame�� Max������ �Ǿ����� [z, x, y��]
	void SetOffsetPos(eRenderEquipment type, Vector3 vOffsetPos = Vector3(3, 10, -8));

	// ������ �̸����� �ؽ��ĸ� �����մϴ�.
	void ChangeTexture(eRenderEquipment type, CString szItemName);

	// ��� �����մϴ�. (���忡�� ������ ȹ���, �������� ������ �������, �ʱ� ������ ������ ��)
	void Equip(ItemInfo* pItem);
	
private:
	CString GetFrameName(ItemInfo* itemInfo);

private:
	FactoryGameObject factory;

	// ������ ��� �����۵�
	vector<ItemInfo*> m_vecEquipedItems;

	// ������ �� ��� �����۵�
	vector<RenderEquipment*> m_vecRenderEquipments;
};

