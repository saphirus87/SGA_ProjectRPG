#pragma once
#include "stdafx.h"

class ScenePicking : public Scene
{
public:
	ScenePicking(CString szName);
	~ScenePicking();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;
};

