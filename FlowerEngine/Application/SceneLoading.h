#pragma once
#include "stdafx.h"

class SceneLoading : public Scene
{
public:
	SceneLoading(CString szName);
	~SceneLoading();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
};

