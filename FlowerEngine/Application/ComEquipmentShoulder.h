#pragma once
#include "stdafx.h"

class EquipmentShoulder;

class ComEquipmentShoulder : public Component
{
public:
	ComEquipmentShoulder(CString szName);
	~ComEquipmentShoulder();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// ����� �������� Export���� �����Ƿ� �������� �����Ͽ� ��ġ�� ������ �ݴϴ�.
	// .X File Export�� Frame�� Max������ �Ǿ����� [z, x, y��]
	void SetOffsetPos(Vector3 vOffsetPosR = Vector3(3, 10, -8));

	EquipmentShoulder* pDataSholder;

private:
	ComRenderSkinnedMesh * m_pAnimation;

	Vector3 m_vOffsetPosR;
	Vector3 m_vOffsetPosL;

	// ��� ���� ����
	GameObject* m_pGOShoulderRight;
	GameObject* m_pGOShoulderLeft;
	ComRenderXMesh* m_pRenderRight;
	ComRenderXMesh* m_pRenderLeft;
};

