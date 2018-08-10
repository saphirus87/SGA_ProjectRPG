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
	// 위치 보정값
	Vector3 m_vOffsetPos;
	// 이 장비의 부모 게임 오브젝트
	GameObject* m_pGOParent;
	// 장비 오브젝트
	GameObject* m_pGOEquipment;
	// 렌더링 구성요소
	ComRenderXMesh* m_pRender;
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
	void ChangeTexture(CString szItemName);

	EquipmentShoulder* pDataSholder;

private:
	map<CString, CString> m_mapTextureName;

	// 캐릭터 애니메이션 구성요소
	ComRenderSkinnedMesh * m_pAnimation;

	vector<RenderEquipment*> m_vecRenderEquipments;

	// 위치 보정값 오른쪽, 왼쪽
	Vector3 m_vOffsetPosR;
	Vector3 m_vOffsetPosL;

	// 어깨 방어구 게임오브젝트 오른쪽, 왼쪽
	GameObject* m_pGOShoulderRight;
	GameObject* m_pGOShoulderLeft;
	GameObject* m_pGOHelmet;

	// 어깨 방어구 렌더링 구성요소 오른쪽, 왼쪽
	ComRenderXMesh* m_pRenderRight;
	ComRenderXMesh* m_pRenderLeft;
	ComRenderXMesh* m_pRenderHelmet;
};

