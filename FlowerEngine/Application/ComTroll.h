#pragma once
#include "stdafx.h"
#include "ComCharacter.h"

class ComTroll : public ComCharacter
{
public:
	ComTroll(CString szName);
	~ComTroll();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetAniEvent();
};

