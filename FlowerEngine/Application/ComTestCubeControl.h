#pragma once
#include "stdafx.h"

class ComTestCubeControl : public Component
{
public:
	ComTestCubeControl(CString szName);
	~ComTestCubeControl();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

