#pragma once
#include "stdafx.h"

class UIControl
{
public:
	unsigned int UID;

protected:
	LPD3DXSPRITE m_pSprite;
	D3DXCOLOR m_Color;

	Vector3 m_Position;
	Vector3 m_Pivot;
	Vector2 m_Size;
	Vector3 m_Scale;

	ComDialog* m_pParent;

	bool m_IsVisible;

public:
	UIControl();
	~UIControl();

	virtual void Awake() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual bool IsOnMouse();

	void SetSprite(LPD3DXSPRITE pSprite) { m_pSprite = pSprite; }
	void SetColor(D3DXCOLOR color) { m_Color = color; }
	void SetPosition(Vector3 pos) { m_Position = pos; }
	void SetPivot(Vector3 pivotPos) { m_Pivot = pivotPos; }
	void SetSize(Vector2 size) { m_Size = size; }
	void SetScale(Vector3 scale) { m_Scale = scale; }
	void SetParent(ComDialog* parent) { m_pParent = parent; }
	void SetIsVisible(bool IsVisible) { m_IsVisible = IsVisible; }

	Vector2 GetSize() { return m_Size; }
};

