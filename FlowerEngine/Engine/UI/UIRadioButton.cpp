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
	if (!m_IsVisible) return;

	for (int i = 0; i < m_vecRadioButton.size(); ++i)
	{
		if (IsOnMouse(i))
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
		RECT rc;
		SetRect(&rc, 0, 0, m_Size.x, m_Size.y);
		Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position + Vector3(0.0f, i *  max(m_Size.y, m_FontSize.y), 0.0f);
		D3DXMATRIXA16 matS, matT;
		D3DXMatrixScaling(&matS, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);

		m_pSprite->SetTransform(&(matS * matT));
		m_pSprite->Draw(m_pButtonTex, &rc, &m_Pivot, &Vector3(0, 0, 0), m_Color);

		if (i == m_CheckedNum)
			m_pSprite->Draw(m_pCheckTex, &rc, &m_Pivot, &Vector3(0, 0, 0), m_Color);

		int textWidth, textHeight;
		textWidth = m_vecRadioButton[i].TextRc.right - m_vecRadioButton[i].TextRc.left;
		textHeight = m_vecRadioButton[i].TextRc.bottom - m_vecRadioButton[i].TextRc.top;
		SetRect(&rc, rc.right, rc.top, rc.right + textWidth, rc.top + textHeight);
		m_pFont->DrawTextW(m_pSprite, m_vecRadioButton[i].Text, m_vecRadioButton[i].Text.GetLength(), &rc, DT_CENTER | DT_VCENTER, m_Color);
	}
}

bool UIRadioButton::IsOnMouse(UINT listNum)
{
	POINT mousePos;
	mousePos.x = Input::GetMousePosition().x;
	mousePos.y = Input::GetMousePosition().y;

	return PtInRect(&m_vecRadioButton[listNum].ButtonRc, mousePos);
}

void UIRadioButton::SetTexture(CString szBoxImg, CString szCheckImg)
{
	m_pButtonTex = Assets::GetTexture(szBoxImg, &m_ImageInfo);
	m_pCheckTex = Assets::GetTexture(szCheckImg, &m_ImageInfo);

	m_Size.x = m_ImageInfo.Width;
	m_Size.y = m_ImageInfo.Height;
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
	SetRect(&button.ButtonRc, pos.x, pos.y, pos.x + m_Size.x, pos.y + m_Size.y);
	//Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position + Vector3(0.0f, max(m_Size.y, m_FontSize.y) * m_vecRadioButton.size(), 0.0f);
	//SetRect(&button.ButtonRc, pos.x, pos.y, pos.x + m_FontSize.x * button.Text.GetLength(), pos.y + m_Size.y);
	//SetRect(&m_ButtonRc, 0, 0, m_Size.x, m_Size.y);
	SetRect(&button.TextRc, button.ButtonRc.right, button.ButtonRc.top, button.ButtonRc.right + m_FontSize.x * button.Text.GetLength(), button.ButtonRc.top + max(m_Size.y, m_FontSize.y));

	m_vecRadioButton.push_back(button);
}
