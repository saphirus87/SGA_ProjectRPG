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
};

