#pragma once
#include "stdafx.h"
#include "ComCharacter.h"

class ComTroll : public ComCharacter, public UIButtonDelegate
{
public:
	ComTroll(CString szName);
	~ComTroll();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// UIButtonDelegate��(��) ���� ��ӵ�
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;

	void SetAniEvent();
};

