#pragma once
#include "stdafx.h"

class SceneLoading : public Scene
{
public:
	SceneLoading(CString szName);
	~SceneLoading();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;
};

