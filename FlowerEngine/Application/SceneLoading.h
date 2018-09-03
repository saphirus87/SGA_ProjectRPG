#pragma once
#include "stdafx.h"

class SceneLoading : public Scene, public UIButtonDelegate
{
public:
	SceneLoading(CString szName);
	~SceneLoading();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;

	// UIButtonDelegate��(��) ���� ��ӵ�
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;
};

