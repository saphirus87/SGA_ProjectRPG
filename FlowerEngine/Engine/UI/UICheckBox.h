#pragma once
#include "stdafx.h"

class UICheckBox : public UIControl
{
	enum eBoxState
	{
		eBoxState_Normal,
		eBoxState_Mouseover,
		eBoxState_Click,
		eBoxState_Count
	};
private:
	LPDIRECT3DTEXTURE9 m_pBoxTex;
	LPDIRECT3DTEXTURE9 m_pCheckTex;
	D3DXIMAGE_INFO m_ImageInfo;

	LPD3DXFONT m_pFont;
	CString m_Text;

	RECT m_BoxRc;
	RECT m_TextRc;

	eBoxState m_BoxState;
	bool m_IsCheck;
	//DWORD m_drawTextFormat;

public:
	UICheckBox();
	~UICheckBox();

	// UIControl을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetTexture(CString szBoxImg, CString szCheckImg);
	void SetText(LPD3DXFONT font, CString szText);
	void SetSize(Vector2 size) { m_Size = size; }

	bool GetIsCheck() { return m_IsCheck; }
};