#pragma once
#include "stdafx.h"
#include "ComCharacter.h"
#include "StatusInfo.h"

class ComObjMap;
class ComFollowTarget;
class IChrState;

// Smallderon의 인공지능(AI)
class ComSmallderonAI : public ComCharacter
{
public:
	ComSmallderonAI(CString szName);
	~ComSmallderonAI();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	//상태 기계
	void Stand() override;
	void Walk(float fDeltaZ) override;
	void Attack1() override;
	void Death() override;

	// 공격할 대상을 찾습니다.
	void FindAttackTarget();
	// 캐릭터가 죽었는지 확인하고 처리합니다.
	void CheckPlayerDeath();

private:
	CTimer* m_pTimerAttack;
};

