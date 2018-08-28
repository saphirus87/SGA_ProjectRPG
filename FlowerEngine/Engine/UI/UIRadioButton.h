#pragma once
#include "stdafx.h"

class UIRadioButton : public UIControl
{
	enum eRadioButtonState
	{
		eRadioButtonState_Normal,
		eRadioButtonState_Mouseover,
		eRadioButtonState_Click,
		eRadioButtonState_Count
	};

	struct RadioButton
	{
		CString Text;
		eRadioButtonState ButtonState;

		RECT ButtonRc;
		RECT TextRc;
	};
private:
	LPDIRECT3DTEXTURE9 m_pButtonTex;
	LPDIRECT3DTEXTURE9 m_pCheckTex;
	D3DXIMAGE_INFO m_ImageInfo;
	LPD3DXFONT m_pFont;
	//RECT m_ButtonRc;

	vector<RadioButton> m_vecRadioButton;

	UINT m_CheckedNum;
	Vector2 m_FontSize;
	//DWORD m_drawTextFormat;

public:
	UIRadioButton();
	~UIRadioButton();

	// UIControl을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool IsOnMouse(UINT listNum);

	void SetTexture(CString szBoxImg, CString szCheckImg);
	void SetFont(LPD3DXFONT font);

	void AddRadioButton(CString szText);

	UINT GetCheckedNum() { return m_CheckedNum; }
};