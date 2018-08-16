#pragma once
#include "stdafx.h"

class ComFollowTarget : public Component
{
public:
	ComFollowTarget(CString szName);
	~ComFollowTarget();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// ���� ���� �ӵ�
	float accellation;
	// �� ���� ����
	Vector3 vForward;
	// ���󰡾� �� Ÿ��
	GameObject * pTarget;
	// Ÿ���� ���󰡴� �� ����
	bool IsFollowing;
	// ���� ���� ����
	bool AbleAttack;

private:
	float m_fLerp;
};

