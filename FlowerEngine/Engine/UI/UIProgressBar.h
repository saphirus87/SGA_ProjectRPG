#pragma once
#include "stdafx.h"

class UIProgressBar : public UIControl
{
private:
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXIMAGE_INFO m_imgInfo;

	UINT m_CurValue;
	UINT m_MaxValue;

	D3DXCOLOR m_MinColor;
	D3DXCOLOR m_MaxColor;

public:
	UIProgressBar();
	~UIProgressBar();

	// UIControl을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetTexture(CString szFileName);

	void SetMaxValue(UINT maxValue) { m_MaxValue = maxValue; }
	void SetCurValue(UINT curValue) { m_CurValue = min(curValue, m_MaxValue); }

	void SetMinColor(D3DXCOLOR minColor) { m_MinColor = minColor; }
	void SetMaxColor(D3DXCOLOR maxColor) { m_MaxColor = maxColor; }
};

