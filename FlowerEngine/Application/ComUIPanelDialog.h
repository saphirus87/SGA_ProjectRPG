#pragma once
#include "stdafx.h"

enum eChr
{
	eChr_Man,
	eChr_Girl
};

class DialogInfo
{
public:
	DialogInfo();
	~DialogInfo();

	eChr eChr;
	CString text;
};

class ComUIPanelDialog : public Component, public IUIButtonDelegate
{
public:
	ComUIPanelDialog(CString szName);
	~ComUIPanelDialog();

	void OnClick(ComUIButton* pSender);

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void AddDialog(eChr chr, int iGroupIndex, CString text);
	void OpenDialog(int DlgGroupIndex);

private:
	GameObject * m_pImgMan;
	GameObject * m_pImgGirl;

	ComUIText* m_pUIText;

	vector<vector<DialogInfo*>> m_vecDialog;
	int m_iCurrentDlgIndex;
	int m_iCurrentDlgGroupIndex;
};

