#include "stdafx.h"
#include "UIButton.h"


UIButton::UIButton()
	: UIControl(),
	  m_ButtonState(eButtonState_Normal)
{
}


UIButton::~UIButton()
{
}

void UIButton::Awake()
{
}

void UIButton::Update()
{
	RECT rc;
	Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;
	SetRect(&rc, pos.x, pos.y, pos.x + m_Size.x, pos.y + m_Size.y);

	POINT mousePos;
	mousePos.x = Input::GetMousePosition().x;
	mousePos.y = Input::GetMousePosition().y;

	if (PtInRect(&rc, mousePos))
	{
		m_ButtonState = eButtonState_Mouseover;
	}
	else
	{
		m_ButtonState = eButtonState_Normal;
	}
}

void UIButton::Render()
{
	RECT rc;
	SetRect(&rc, 0, 0, m_Size.x, m_Size.y);
	Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;

	m_pSprite->Draw(m_Textures[m_ButtonState], &rc, &m_Pivot, &pos, m_Color);
}

void UIButton::SetTexture(CString szNormalImg, CString szMouseoverImg, CString szClickImg)
{
	m_Textures[eButtonState_Normal] = Assets::GetTexture(szNormalImg, &m_ImageInfo);
	m_Textures[eButtonState_Mouseover] = Assets::GetTexture(szMouseoverImg, &m_ImageInfo);
	m_Textures[eButtonState_Click] = Assets::GetTexture(szClickImg, &m_ImageInfo);

	m_Size.x = m_ImageInfo.Width;
	m_Size.y = m_ImageInfo.Height;
}
