#pragma once
#include "stdafx.h"

class ComCharacterUI : public Component
{
public:
	ComCharacterUI(CString szName);
	~ComCharacterUI();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

