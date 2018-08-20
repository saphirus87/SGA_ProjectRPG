#pragma once
#include "stdafx.h"

class UIText : public UIControl
{
private:
	LPD3DXFONT m_pFont;
	CString m_Text;
	DWORD m_drawTextFormat;

public:
	UIText();
	~UIText();

	// UIControl을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetText(LPD3DXFONT font, CString szText) { m_pFont = font; m_Text = szText; }
	void SetSize(Vector2 size) { m_Size = size; }
};

