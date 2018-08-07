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

private:
	ComRenderSkinnedMesh * m_pAnimation;

	// ��� ���� ����
	ComRenderXMesh* m_pSholderRight;
	ComRenderXMesh* m_pSholderLeft;
	Matrix4x4 m_matSholderRight;
	Matrix4x4 m_matSholderLeft;
};

