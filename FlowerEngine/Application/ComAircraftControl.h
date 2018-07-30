#pragma once
#include "stdafx.h"

class ComAircraft;
class ComShooting;
class ComUIPanelDialog;

class ComAircraftControl : public Component
{
public:
	ComAircraftControl(CString szName);
	~ComAircraftControl();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// ���� ��ġ�� ���� �� (�ǰݽ�)
	virtual void OnTriggerEnter(ComCollider &collider) override;

private:
	void Move();
	void Shooting();
	void UseSkill1();	// ��Ƽ� ���
	void UseSkill2();	// Ÿ�� ���� �ٶ󺸱�

private:
	ComAircraft* m_pAircraft;
	ComShooting* m_pShooting;
	
	CTimer* m_pTimer;

	int m_iCurrentSkill2Index;	// ��ų2 �̻����� ���� �ε���
	
	Vector3* m_pos;				// ����� ��ġ
	Vector3 m_vecForward;		// �� ���� ����
	bool m_isMoving;			// ������ ����

};

