#pragma once
#include "stdafx.h"

class ComEquipmentWeapon : public Component
{
public:
	ComEquipmentWeapon(CString szName);
	~ComEquipmentWeapon();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetOffsetPos(Vector3 vOffsetPosR = Vector3(3, 10, -8));

private:
	// 캐릭터 애니메이션 구성요소
	ComRenderSkinnedMesh * m_pAnimation;

	// 위치 보정값 오른쪽, 왼쪽
	Vector3 m_vOffsetPosR;
	Vector3 m_vOffsetPosL;
	Vector3 m_vOffsetPosLShield;

	// 무기 또는 들것 게임오브젝트 오른쪽, 왼쪽
	GameObject* m_pGOWeaponRight;
	GameObject* m_pGOWeaponLeft;
	GameObject* m_pGOShieldLeft; // 왼손 방패

	// 무기 또는 들것 렌더링 구성요소 오른쪽, 왼쪽
	ComRenderXMesh* m_pRenderRight;
	ComRenderXMesh* m_pRenderLeft;
	ComRenderXMesh* m_pRenderShieldLeft; // 왼손 방패
};

