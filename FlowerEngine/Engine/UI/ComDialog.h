#pragma once
#include "stdafx.h"

class UIControl;
class UIImage;
class UIText;
class UIButton;
class UIButtonDelegate;
class UIProgressBar;
class UICheckBox;
class UIRadioButton;

class ComDialog : public Component
{
private:
	LPD3DXSPRITE m_pSprite;
	map<UINT, UIControl*> m_Controls;

	DWORD m_ToggleKey;
	bool m_IsVisible;
	RECT m_DragArea;
	bool m_Moveable;
	bool m_IsDrag;

public:
	ComDialog(CString szName);
	~ComDialog();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	bool IsOnMouse();
	void SetIsVisible(bool IsVisible) { m_IsVisible = IsVisible; }
	void SetDragArea(RECT Area) { m_DragArea = Area; }
	void SetMoveable(bool Moveable) { m_Moveable = Moveable; }
	void SetToggleKey(DWORD ToggleKey) { m_ToggleKey = ToggleKey; }

	void AddImage(UINT id, CString szFileName, bool isFullScreen = false);
	void AddText(UINT id, LPD3DXFONT pFont, CString szText);
	void AddButton(UINT id, CString szNormalImg, CString szMouseoverImg, CString szClickImg, UIButtonDelegate* pDelegate, CString szButtonName);
	void AddBtnInvenIcon(UINT id, CString szNormalImg, CString szMouseoverImg, CString szClickImg, UIButtonDelegate* pDelegate, CString szButtonName);
	void AddProgressBar(UINT id, CString szFileName);
	void AddCheckBox(UINT id, CString szBoxImg, CString szCheckImg, LPD3DXFONT pFont, CString szText);
	void AddRadioButton(UINT id, CString szButtonImg, CString szCheckImg, LPD3DXFONT pFont, CString szText);

	// Getter
	UIImage* GetImage(UINT id) { return (UIImage*)m_Controls[id]; }
	UIText* GetText(UINT id) { return (UIText*)m_Controls[id]; }
	UIButton* GetButton(UINT id) { return (UIButton*)m_Controls[id]; }
	UIProgressBar* GetProgressBar(UINT id) { return (UIProgressBar*)m_Controls[id]; }
	UICheckBox* GetCheckBox(UINT id) { return (UICheckBox*)m_Controls[id]; }
	UIRadioButton* GetRadioButton(UINT id) { return (UIRadioButton*)m_Controls[id]; }
};

