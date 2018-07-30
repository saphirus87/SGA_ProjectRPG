#pragma once
#include "stdafx.h"

class ComUI : public Component
{
public:
	ComUI(CString szName);
	virtual ~ComUI();
	
	void DrawBorder();

	// width, height
	D3DXVECTOR2		size;
	D3DCOLOR		color;

	bool			IsDrawBorder;

protected:
	Device9			pDevice9;
	LPD3DXSPRITE	m_pSprite;
	
	D3DXVECTOR3		m_pivot;
	RECT			m_screenRect;
};

