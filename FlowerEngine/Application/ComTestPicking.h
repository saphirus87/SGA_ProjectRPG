#pragma once
#include "stdafx.h"

class ComTestPicking : public Component
{
public:
	ComTestPicking(CString szName);
	~ComTestPicking();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

