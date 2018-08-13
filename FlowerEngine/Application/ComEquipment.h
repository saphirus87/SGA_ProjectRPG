#pragma once
#include "stdafx.h"

class EquipmentShoulder;
class ComRenderEquipment;

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

	// 뼈이름
	LPCSTR szFrameName;
	// 위치 보정값
	Vector3 m_vOffsetPos;
	// 이 장비의 부모 게임 오브젝트
	GameObject* m_pGOParent;
	// 장비 오브젝트
	GameObject* m_pGOEquipment;
	// 렌더링 구성요소
	ComRenderEquipment* m_pRender;
	// 애니메이션 포인터
	ComRenderSkinnedMesh * m_pAnimation;
};

class ComEquipment : public Component
{
public:
	ComEquipment(CString szName);
	~ComEquipment();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 어깨방어구 장착뼈가 Export되지 않으므로 보정값을 설정하여 위치를 보정해 줍니다.
	// .X File Export시 Frame이 Max축으로 되어있음 [z, x, y축]
	void SetOffsetPos(Vector3 vOffsetPosR = Vector3(3, 10, -8));

	// 아이템 이름으로 텍스쳐를 변경합니다.
	void ChangeTexture(eEquipment type, CString szItemName);

	EquipmentShoulder* pDataSholder;

private:
	map<CString, CString> m_mapTextureName;

	vector<RenderEquipment*> m_vecRenderEquipments;
};

