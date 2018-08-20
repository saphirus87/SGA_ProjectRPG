#pragma once
#include "stdafx.h"

class UIImage : public UIControl
{
private:
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXIMAGE_INFO m_imgInfo;

public:
	UIImage();
	~UIImage();

	// UIControl��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetTexture(CString szFileName);
};

