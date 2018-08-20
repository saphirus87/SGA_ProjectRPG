#pragma once
#include "stdafx.h"

class UIButton : public UIControl
{
	enum eButtonState
	{
		eButtonState_Normal,
		eButtonState_Mouseover,
		eButtonState_Click,
		eButtonState_Count
	};
private:
	LPDIRECT3DTEXTURE9 m_Textures[eButtonState_Count];
	D3DXIMAGE_INFO m_ImageInfo;
	eButtonState m_ButtonState;

public:
	UIButton();
	~UIButton();

	// UIControl을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetTexture(CString szNormalImg, CString szMouseoverImg, CString szClickImg);
};

