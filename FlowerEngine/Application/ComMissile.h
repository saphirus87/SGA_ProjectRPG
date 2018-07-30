#pragma once
#include "stdafx.h"

class ComShooting;
class ComAircraftControl;
class ComAircraft;

class ComMissile : public Component
{
public:
	ComMissile(CString szName);
	~ComMissile();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void OnTriggerEnter(ComCollider &collider) override;

	void Set(ComShooting* pShooting, float fRange);
	void SetDirection(Vector3 vDir, float fSpeed);
	inline void SetStartPos(Vector3 vStartPos) { m_posStart = vStartPos; }

protected:
	ComShooting * m_pShooting;
	ComAircraft* m_pAircraft;

	CString m_szShooterName;		// �� �̻����� �߻��� ���� ������Ʈ�� �̸�

	Vector3 m_vDirection;		// ���ư� ����
	Vector3 m_posStart;			// ���� ��ġ (���ư� �Ÿ� ����ϱ� ����)
	
	float m_fSpeed;				// �ӵ�
	float m_fFlyLength;			// ���ư� �Ÿ�
	float m_fFlyLengthMax;		// �����Ÿ� (�ִ� ���ư� �� �ִ� �Ÿ�)
};

