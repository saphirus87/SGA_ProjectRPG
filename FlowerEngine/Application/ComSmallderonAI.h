#pragma once
#include "stdafx.h"
#include "ComChrControl.h"
#include "StatusInfo.h"

class ComObjMap;
class ComFollowTarget;
class IChrState;
class ComCharacter;

// Smallderon의 인공지능(AI)
class ComSmallderonAI : public ComChrControl
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

private:
	ComCharacter* m_pCharacter;
	CTimer* m_pTimerAttack;
};

