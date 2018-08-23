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

	// �� ���� ����
	Vector3 vDir;
	// �ӵ�
	float fMoveSpeed;
	// ���󰡾� �� Ÿ��
	GameObject * pTarget;
	// Ÿ���� ���󰡴� �� ����
	bool IsFollowing;
	// ���� ���� ����
	bool AbleAttack;

	// ���� ���� �ӵ�
	float accellation;

private:
	float m_fLerp;
};

