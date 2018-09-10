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

	// 폰트와 텍스트 동시 설정(텍스트 길이 변동으로 사이즈 변경이 필요한 경우에도 사용) 
	void SetText(LPD3DXFONT font, CString szText);
	// 텍스트 내용 수정(사이즈 변경이 없으므로 사이즈 이상의 텍스트는 짤림)
	void SetText(CString szText) { m_Text = szText; }
	// 폰트 설정
	void SetFont(LPD3DXFONT font);
	// 텍스트 포맷(텍스트를 그릴때 상하좌우 정렬)
	void SetDrawFormat(DWORD format) { m_drawTextFormat = format; }
	// 텍스트 내에 한글의 갯수를 반환(크기 설정에 필요)
	int GetHangulNum();
};

