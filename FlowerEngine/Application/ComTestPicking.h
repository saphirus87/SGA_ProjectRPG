#pragma once
#include "stdafx.h"

class ComTestPicking : public Component
{
public:
	ComTestPicking(CString szName);
	~ComTestPicking();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

