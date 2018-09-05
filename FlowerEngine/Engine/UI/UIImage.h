#pragma once
#include "stdafx.h"

class UIImage : public UIControl
{
private:
	Texture pTexture;
	D3DXIMAGE_INFO m_imgInfo;

public:
	UIImage();
	~UIImage();

	// UIControl을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetTexture(CString szFileName, bool isFullScreen = false);
};

