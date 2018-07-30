#pragma once
#include "stdafx.h"

class ComUIText : public ComUI
{
public:
	ComUIText(CString szName);
	~ComUIText();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// �������� ����
	CString			text;
		
	// �ܰ��� �׸��� ����
	bool			IsDrawBoarder;	

	void SetFont(Assets::FontType type);


private:
	LPD3DXFONT		m_pFont;
	DWORD			m_drawTextFormat;
};	

