#pragma once
#include "stdafx.h"

class ComUIImage : public ComUI
{
public:
	ComUIImage(CString szName);
	~ComUIImage();

	// ComUI��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetTexture(CString szFileName);
	
	// ��ü ȭ�� ũ��� �����մϴ�.
	void SetFullScreen();

private:
	Texture m_pTex;
	D3DXIMAGE_INFO m_imgInfo;
	bool m_bMirrored;
};

