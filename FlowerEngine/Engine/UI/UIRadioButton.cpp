#include "stdafx.h"
#include "UIRadioButton.h"

UIRadioButton::UIRadioButton()
	: UIControl(),
	  m_pButtonTex(NULL),
	  m_pCheckTex(NULL),
	  m_pFont(NULL),
	  m_CheckedNum(0)
{
}


UIRadioButton::~UIRadioButton()
{

}

void UIRadioButton::Awake()
{

}

void UIRadioButton::Update()
{
	POINT mousePos;
	mousePos.x = Input::GetMousePosition().x;
	mousePos.y = Input::GetMousePosition().y;

	for (int i = 0; i < m_vecRadioButton.size(); ++i)
	{
		if (PtInRect(&m_vecRadioButton[i].ButtonRc, mousePos))
		{
			if (Input::ButtonPress(VK_LBUTTON))
			{
				if (m_vecRadioButton[i].ButtonState == eRadioButtonState_Mouseover)
					m_vecRadioButton[i].ButtonState = eRadioButtonState_Click;
			}
			else
			{
				if (m_vecRadioButton[i].ButtonState == eRadioButtonState_Click)
				{
					m_CheckedNum = i;
				}

				m_vecRadioButton[i].ButtonState = eRadioButtonState_Mouseover;
			}
		}
		else
		{
			if (Input::ButtonPress(VK_LBUTTON));
			else m_vecRadioButton[i].ButtonState = eRadioButtonState_Normal;
		}
	}
	
}

void UIRadioButton::Render()
{
	if (m_pButtonTex == NULL) return;

	for (int i = 0; i < m_vecRadioButton.size(); ++i)
	{
		Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position + Vector3(0.0f, i *  max(m_Size.y, m_FontSize.y), 0.0f);

		m_pSprite->Draw(m_pButtonTex, &m_ButtonRc, &m_Pivot, &pos, m_Color);

		if (i == m_CheckedNum)
			m_pSprite->Draw(m_pCheckTex, &m_ButtonRc, &m_Pivot, &pos, m_Color);

		m_pFont->DrawTextW(m_pSprite, m_vecRadioButton[i].Text, m_vecRadioButton[i].Text.GetLength(), &m_vecRadioButton[i].TextRc, DT_CENTER | DT_VCENTER, m_Color);
	}
}

void UIRadioButton::SetTexture(CString szBoxImg, CString szCheckImg)
{
	m_pButtonTex = Assets::GetTexture(szBoxImg, &m_ImageInfo);
	m_pCheckTex = Assets::GetTexture(szCheckImg, &m_ImageInfo);

	m_Size.x = m_ImageInfo.Width;
	m_Size.y = m_ImageInfo.Height;

	SetRect(&m_ButtonRc, 0, 0, m_Size.x, m_Size.y);
}

void UIRadioButton::SetFont(LPD3DXFONT font)
{
	m_pFont = font;

	D3DXFONT_DESCW fontDesc;
	m_pFont->GetDescW(&fontDesc);

	m_FontSize.x = fontDesc.Width;
	m_FontSize.y = fontDesc.Height;
}

void UIRadioButton::AddRadioButton(CString szText)
{
	RadioButton button;

	button.Text = szText;
	button.ButtonState = eRadioButtonState_Normal;
	
	Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position + Vector3(0.0f, max(m_Size.y, m_FontSize.y) * m_vecRadioButton.size(), 0.0f);
	SetRect(&button.ButtonRc, pos.x, pos.y, pos.x + m_FontSize.x * button.Text.GetLength(), pos.y + m_Size.y);
	SetRect(&button.TextRc, button.ButtonRc.right, pos.y, button.ButtonRc.right + m_FontSize.x * button.Text.GetLength(), pos.y + max(m_Size.y, m_FontSize.y));

	m_vecRadioButton.push_back(button);
}
