#pragma once
#include "stdafx.h"
#include "ComCharacter.h"

class ComTroll01 : public ComCharacter
{
public:
	ComTroll01(CString szName);
	~ComTroll01();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

