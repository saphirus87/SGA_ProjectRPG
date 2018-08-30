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

	void SetText(LPD3DXFONT font, CString szText);
	void SetText(CString szText) { m_Text = szText; }
	void SetFont(LPD3DXFONT font);
	void SetDrawFormat(DWORD format) { m_drawTextFormat = format; }
	void SetSize(Vector2 size) { m_Size = size; }
};

