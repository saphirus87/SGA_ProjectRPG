#pragma once
#include "stdafx.h"

class UIText : public UIControl
{
private:
	CString m_Text;

public:
	UIText();
	~UIText();

	// UIControl��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetText(CString szText) { m_Text = szText; }
};

