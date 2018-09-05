#pragma once
#include "stdafx.h"

class ComUIImage : public ComUI
{
public:
	ComUIImage(CString szName);
	~ComUIImage();

	// ComUI을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetTexture(CString szFileName);
	
	// 전체 화면 크기로 지정합니다.
	void SetFullScreen();

private:
	Texture m_pTex;
	D3DXIMAGE_INFO m_imgInfo;
	bool m_bMirrored;
};

