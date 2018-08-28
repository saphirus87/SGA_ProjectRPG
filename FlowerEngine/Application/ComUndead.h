#pragma once
#include "stdafx.h"
#include "ComCharacter.h"

class ChrState;

class ComUndead : public ComCharacter
{
public:
	ComUndead(CString szName);
	~ComUndead();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetAniEvent();
};

