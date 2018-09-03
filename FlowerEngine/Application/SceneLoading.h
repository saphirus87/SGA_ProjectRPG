#pragma once
#include "stdafx.h"

class SceneLoading : public Scene, public UIButtonDelegate
{
public:
	SceneLoading(CString szName);
	~SceneLoading();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;

	// UIButtonDelegate을(를) 통해 상속됨
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;
};

