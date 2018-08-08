#pragma once
#include "stdafx.h"

class EquipmentShoulder;

class ComEquipmentShoulder : public Component
{
public:
	ComEquipmentShoulder(CString szName);
	~ComEquipmentShoulder();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 어깨방어구 장착뼈가 Export되지 않으므로 보정값을 설정하여 위치를 보정해 줍니다.
	// .X File Export시 Frame이 Max축으로 되어있음 [z, x, y축]
	void SetOffsetPos(Vector3 vOffsetPosR = Vector3(3, 10, -8));

	EquipmentShoulder* pDataSholder;

private:
	ComRenderSkinnedMesh * m_pAnimation;

	Vector3 m_vOffsetPosR;
	Vector3 m_vOffsetPosL;

	// 어깨 장착 관련
	GameObject* m_pGOShoulderRight;
	GameObject* m_pGOShoulderLeft;
	ComRenderXMesh* m_pRenderRight;
	ComRenderXMesh* m_pRenderLeft;
};

