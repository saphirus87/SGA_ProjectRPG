#pragma once
#include "stdafx.h"

class UIControl
{
protected:
	LPD3DXSPRITE m_pSprite;
	D3DXCOLOR m_Color;

	Vector3 m_Position;
	Vector3 m_Pivot;
	Vector2 m_Size;

	ComDialog* m_pParent;

public:
	UIControl();
	~UIControl();

	virtual void Awake() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	void SetSprite(LPD3DXSPRITE pSprite) { m_pSprite = pSprite; }
	void SetColor(D3DXCOLOR color) { m_Color = color; }
	void SetPosition(Vector3 pos) { m_Position = pos; }
	void SetPivot(Vector3 pivotPos) { m_Pivot = pivotPos; }
	void SetParent(ComDialog* parent) { m_pParent = parent; }
};

