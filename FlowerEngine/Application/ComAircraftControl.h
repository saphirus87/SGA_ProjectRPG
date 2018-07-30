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

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 몸통 박치기 했을 때 (피격시)
	virtual void OnTriggerEnter(ComCollider &collider) override;

private:
	void Move();
	void Shooting();
	void UseSkill1();	// 모아서 잡기
	void UseSkill2();	// 타겟 방향 바라보기

private:
	ComAircraft* m_pAircraft;
	ComShooting* m_pShooting;
	
	CTimer* m_pTimer;

	int m_iCurrentSkill2Index;	// 스킬2 미사일의 현재 인덱스
	
	Vector3* m_pos;				// 비행기 위치
	Vector3 m_vecForward;		// 앞 방향 벡터
	bool m_isMoving;			// 움직임 여부

};

