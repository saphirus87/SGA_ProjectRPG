#include "stdafx.h"
#include "UIText.h"

UIText::UIText()
	: UIControl(),
	m_pFont(NULL),
	m_drawTextFormat(DT_CENTER | DT_VCENTER)
{
}


UIText::~UIText()
{

}

void UIText::Awake()
{

}

void UIText::Update()
{

}

void UIText::Render()
{
	RECT rc;
	Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;
	SetRect(&rc, pos.x, pos.y, pos.x + m_Size.x, pos.y + m_Size.y);

	m_pFont->DrawTextW(m_pSprite, m_Text, m_Text.GetLength(), &rc, m_drawTextFormat, m_Color);
}
