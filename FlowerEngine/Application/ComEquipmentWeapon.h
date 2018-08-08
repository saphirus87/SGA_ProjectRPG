#pragma once
#include "stdafx.h"

class ComEquipmentWeapon : public Component
{
public:
	ComEquipmentWeapon(CString szName);
	~ComEquipmentWeapon();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetOffsetPos(Vector3 vOffsetPosR = Vector3(3, 10, -8));

private:
	// ĳ���� �ִϸ��̼� �������
	ComRenderSkinnedMesh * m_pAnimation;

	// ��ġ ������ ������, ����
	Vector3 m_vOffsetPosR;
	Vector3 m_vOffsetPosL;

	// ���� �Ǵ� ��� ���ӿ�����Ʈ ������, ����
	GameObject * m_pGOWeaponRight;
	GameObject* m_pGOWeaponLeft;

	// ���� �Ǵ� ��� ������ ������� ������, ����
	ComRenderXMesh* m_pRenderRight;
	ComRenderXMesh* m_pRenderLeft;
};

