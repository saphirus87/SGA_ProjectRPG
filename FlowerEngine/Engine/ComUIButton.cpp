#include "stdafx.h"
#include "ComUIButton.h"


ComUIButton::ComUIButton(CString szName) : 
	ComUI(szName),
	m_buttonState(NORMAL)
{
}


ComUIButton::~ComUIButton()
{
}

void ComUIButton::Awake()
{
}

void ComUIButton::Update()
{
	POINT mousePoint;
	GetCursorPos(&mousePoint);
	ScreenToClient(GetForegroundWindow(), &mousePoint);

	Vector3 pos = gameObject->transform->GetPosition();
	
	SetRect(&m_screenRect, pos.x, pos.y, pos.x + size.x, pos.y + size.y);

	if (PtInRect(&m_screenRect, mousePoint))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_buttonState == MOUSEOVER)
			{
				m_buttonState = SELECTED;
			}
		}
		else
		{
			if (m_buttonState == SELECTED)
			{
				if (m_pDelegate)
					m_pDelegate->OnClick(this);
			}
			m_buttonState = MOUSEOVER;
		}
	}
	else
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{

		}
		else
		{
			m_buttonState = NORMAL;
		}
	}
}

void ComUIButton::Render()
{
	DrawBorder();
}
