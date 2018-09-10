#pragma once
#include "stdafx.h"

class UIProgressBar : public UIControl
{
private:
	LPDIRECT3DTEXTURE9 pTexture;
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

	// 프로그래스바의 이미지 텍스처 설정
	void SetTexture(CString szFileName);
	// 프로그래스바의 최대값 및 현재 진행 값 설정
	void SetMaxValue(UINT maxValue) { m_MaxValue = maxValue; }
	void SetCurValue(UINT curValue) { m_CurValue = min(curValue, m_MaxValue); }
	// 프로그래스바 진행에 따른 색 설정(최소,최대값에 선형보간을 이용해서 현재 진행도에 맞는 색상 표시)
	void SetMinColor(D3DXCOLOR minColor) { m_MinColor = minColor; }
	void SetMaxColor(D3DXCOLOR maxColor) { m_MaxColor = maxColor; }
};

