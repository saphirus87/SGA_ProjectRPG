#pragma once
#include "stdafx.h"

class UIButton;

class UIButtonDelegate
{
public:
	virtual void OnClick(UIButton* pSender) = 0;
	virtual void OnPress(UIButton* pSender) = 0;
};

class UIButton : public UIControl
{
	enum eButtonState
	{
		eButtonState_Normal,
		eButtonState_Mouseover,
		eButtonState_Click,
		eButtonState_Press,
		eButtonState_Count
	};
private:
	LPDIRECT3DTEXTURE9 m_Textures[eButtonState_Count];
	D3DXIMAGE_INFO m_ImageInfo;
	UIButtonDelegate* m_pDelegate;
	eButtonState m_ButtonState;
	CString m_szButtonName;

	float m_PressTimer;

public:
	UIButton(UIButtonDelegate* pDelegate, CString buttonName);
	~UIButton();

	// UIControl을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetTexture(CString szNormalImg, CString szMouseoverImg, CString szClickImg);
	CString GetButtonName() { return m_szButtonName; }
};

