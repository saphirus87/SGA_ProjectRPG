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

	// 뼈이름
	LPCSTR szFrameName;
	// 위치 보정값
	Vector3 m_vOffsetPos;
	// 이 장비의 부모 게임 오브젝트
	GameObject* m_pGOParent;
	// 장비 오브젝트
	GameObject* m_pGOEquipment;
	// 렌더링 구성요소
	ComEquipment* m_pRender;
	// 애니메이션 포인터
	ComRenderSkinnedMesh * m_pAnimation;
};

class ComChrEquipment : public Component
{
public:
	ComChrEquipment(CString szName);
	~ComChrEquipment();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 어깨방어구 장착뼈가 Export되지 않으므로 보정값을 설정하여 위치를 보정해 줍니다.
	// .X File Export시 Frame이 Max축으로 되어있음 [z, x, y축]
	void SetOffsetPos(eRenderEquipment type, Vector3 vOffsetPos = Vector3(3, 10, -8));

	// 아이템 이름으로 텍스쳐를 변경합니다.
	void ChangeTexture(eRenderEquipment type, CString szItemName);

	// 장비를 장착합니다. (월드에서 아이템 획득시, 서버에서 데이터 받은경우, 초기 아이템 장착시 등)
	void Equip(ItemInfo* pItem);
	
private:
	CString GetFrameName(ItemInfo* itemInfo);

private:
	FactoryGameObject factory;

	// 장착된 장비 아이템들
	vector<ItemInfo*> m_vecEquipedItems;

	// 렌더링 할 장비 아이템들
	vector<RenderEquipment*> m_vecRenderEquipments;
};

