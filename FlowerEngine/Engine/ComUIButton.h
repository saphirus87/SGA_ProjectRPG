#pragma once
#include "stdafx.h"

class ComUIButton;

class IUIButtonDelegate
{
public:
	virtual void OnClick(ComUIButton* pSender) = 0;
};

class ComUIButton : public ComUI
{
public:
	enum ButtonState
	{
		NORMAL,
		MOUSEOVER,
		SELECTED,
		COUNT
	};

	ComUIButton(CString szName);
	~ComUIButton();

	// ComUI을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	IUIButtonDelegate * m_pDelegate;

private:
	ButtonState m_buttonState;
};

