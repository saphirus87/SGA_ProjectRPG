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

	EquipmentShoulder* pDataSholder;
};

