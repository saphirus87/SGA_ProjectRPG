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
};

