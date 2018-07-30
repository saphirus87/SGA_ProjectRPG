#pragma once
#include "stdafx.h"

class ComUIText : public ComUI
{
public:
	ComUIText(CString szName);
	~ComUIText();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 렌더링할 글자
	CString			text;
		
	// 외각선 그리기 여부
	bool			IsDrawBoarder;	

	void SetFont(Assets::FontType type);


private:
	LPD3DXFONT		m_pFont;
	DWORD			m_drawTextFormat;
};	

