#pragma once
#include "stdafx.h"

class UIControl;
class UIImage;
class UIText;
class UIButton;
class UIProgressBar;

class ComDialog : public Component
{
private:
	LPD3DXSPRITE m_pSprite;
	map<UINT, UIControl*> m_Controls;

public:
	ComDialog(CString szName);
	~ComDialog();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void AddImage(UINT id, CString szFileName);
	void AddText(UINT id, CString szText);
	void AddButton(UINT id, CString szNormalImg, CString szMouseoverImg, CString szClickImg);
	void AddProgressBar(UINT id, CString szFileName);

	// Getter
	UIButton* GetButton(UINT id) { return (UIButton*)m_Controls[id]; }
	UIProgressBar* GetProgressBar(UINT id) { return (UIProgressBar*)m_Controls[id]; }
};

