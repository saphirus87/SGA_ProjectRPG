#pragma once
#include "stdafx.h"

class ComTestCubeControl : public Component
{
public:
	ComTestCubeControl(CString szName);
	~ComTestCubeControl();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

