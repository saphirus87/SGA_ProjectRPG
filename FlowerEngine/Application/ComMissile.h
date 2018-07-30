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

	// Component을(를) 통해 상속됨
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

	CString m_szShooterName;		// 이 미사일을 발사한 게임 오브젝트의 이름

	Vector3 m_vDirection;		// 날아갈 방향
	Vector3 m_posStart;			// 시작 위치 (날아간 거리 계산하기 위해)
	
	float m_fSpeed;				// 속도
	float m_fFlyLength;			// 날아간 거리
	float m_fFlyLengthMax;		// 사정거리 (최대 날아갈 수 있는 거리)
};

