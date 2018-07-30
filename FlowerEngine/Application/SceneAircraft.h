#pragma once
#include "stdafx.h"

class SceneAircraft : public Scene
{
public:
	SceneAircraft(CString szName);
	~SceneAircraft();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;

	void CreateEnermy(float fPos);
};

