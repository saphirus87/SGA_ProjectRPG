#pragma once
#include "stdafx.h"

class ComCharacterUI : public Component
{
public:
	ComCharacterUI(CString szName);
	~ComCharacterUI();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

