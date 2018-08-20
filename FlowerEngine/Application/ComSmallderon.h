#pragma once
#include "stdafx.h"
#include "ComChrControl.h"

class ComObjMap;
class ComFollowTarget;
class IChrState;

// Smallderon의 인공지능(AI)
class ComSmallderon : public ComChrControl
{
public:
	ComSmallderon(CString szName);
	~ComSmallderon();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	//상태 기계
	void Stand() override;
	void Walk(float fDeltaZ) override;
	void Attack1() override;
};

