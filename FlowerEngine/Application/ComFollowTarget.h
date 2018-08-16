#pragma once
#include "stdafx.h"

class ComFollowTarget : public Component
{
public:
	ComFollowTarget(CString szName);
	~ComFollowTarget();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 선형 보간 속도
	float accellation;
	// 앞 방향 벡터
	Vector3 vForward;
	// 따라가야 할 타겟
	GameObject * pTarget;
	// 타겟을 따라가는 중 여부
	bool IsFollowing;
	// 공격 가능 여부
	bool AbleAttack;

private:
	float m_fLerp;
};

