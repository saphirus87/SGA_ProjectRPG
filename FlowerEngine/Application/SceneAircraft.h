#pragma once
#include "stdafx.h"

class SceneAircraft : public Scene
{
public:
	SceneAircraft(CString szName);
	~SceneAircraft();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;

	void CreateEnermy(float fPos);
};

