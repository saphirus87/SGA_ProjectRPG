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

	// UIControl��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// ��Ʈ�� �ؽ�Ʈ ���� ����(�ؽ�Ʈ ���� �������� ������ ������ �ʿ��� ��쿡�� ���) 
	void SetText(LPD3DXFONT font, CString szText);
	// �ؽ�Ʈ ���� ����(������ ������ �����Ƿ� ������ �̻��� �ؽ�Ʈ�� ©��)
	void SetText(CString szText) { m_Text = szText; }
	// ��Ʈ ����
	void SetFont(LPD3DXFONT font);
	// �ؽ�Ʈ ����(�ؽ�Ʈ�� �׸��� �����¿� ����)
	void SetDrawFormat(DWORD format) { m_drawTextFormat = format; }
	// �ؽ�Ʈ ���� �ѱ��� ������ ��ȯ(ũ�� ������ �ʿ�)
	int GetHangulNum();
};

