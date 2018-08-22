#pragma once
#include "stdafx.h"
#include "ComCharacter.h"

class ChrState;

class ComUndead01 : public ComCharacter
{
public:
	ComUndead01(CString szName);
	~ComUndead01();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// UI ����
	void UpdateHPBar() override;

private:
	UIProgressBar * m_pHPBar;
};

