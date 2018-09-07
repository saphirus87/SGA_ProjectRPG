#include "stdafx.h"
#include "UIControl.h"


UIControl::UIControl()
	:UID(0),
	m_pSprite(NULL),
	m_pParent(NULL),
	m_Color(1.0f, 1.0f, 1.0f, 1.0f),
	m_Position(0.0f, 0.0f, 0.0f),
	m_Pivot(0.0f, 0.0f, 0.0f),
	m_Scale(1.0f, 1.0f, 1.0f),
	m_Size(0, 0),
	m_IsVisible(true)
{
}


UIControl::~UIControl()
{
}

bool UIControl::IsOnMouse()
{
	RECT rc;
	// parent Pos = ComDialog의 위치 + 자신의 위치
	Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;
	SetRect(&rc, pos.x, pos.y, pos.x + (m_Size.x * m_Scale.x), pos.y + (m_Size.y * m_Scale.y));

	POINT mousePos;
	mousePos.x = Input::GetMousePosition().x;
	mousePos.y = Input::GetMousePosition().y;

	return PtInRect(&rc, mousePos);
}
