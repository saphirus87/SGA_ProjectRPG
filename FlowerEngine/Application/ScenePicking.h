#pragma once
#include "stdafx.h"

class ScenePicking : public Scene
{
public:
	ScenePicking(CString szName);
	~ScenePicking();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
};

