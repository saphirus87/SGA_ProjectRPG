#include "stdafx.h"
#include "UICheckBox.h"

UICheckBox::UICheckBox()
	: UIControl(),
	m_pBoxTex(NULL),
	m_pCheckTex(NULL),
	m_pFont(NULL),
	m_BoxState(eBoxState_Normal),
	m_IsCheck(false)
{
}


UICheckBox::~UICheckBox()
{

}

void UICheckBox::Awake()
{

}

void UICheckBox::Update()
{
	if (!m_IsVisible) return;

	if (IsOnMouse())
	{
		if (Input::ButtonPress(VK_LBUTTON))
		{
			if (m_BoxState == eBoxState_Mouseover)
				m_BoxState = eBoxState_Click;
		}
		else
		{
			if (m_BoxState == eBoxState_Click)
				m_IsCheck = !m_IsCheck;

			m_BoxState = eBoxState_Mouseover;
		}
	}
	else
	{
		if (Input::ButtonPress(VK_LBUTTON));
		else m_BoxState = eBoxState_Normal;
	}
}

void UICheckBox::Render()
{
	if (!m_IsVisible) return;

	if (m_pBoxTex)
	{
		RECT rc;
		SetRect(&rc, 0, 0, m_Size.x, m_Size.y);
		Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;
		D3DXMATRIXA16 matS, matT;
		D3DXMatrixScaling(&matS, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);

		m_pSprite->SetTransform(&(matS * matT));
		m_pSprite->Draw(m_pBoxTex, &m_BoxRc, &m_Pivot, &Vector3(0, 0, 0), m_Color);

		if (m_IsCheck)
			m_pSprite->Draw(m_pCheckTex, &m_BoxRc, &m_Pivot, &Vector3(0, 0, 0), m_Color);
	}

	m_pFont->DrawTextW(m_pSprite, m_Text, m_Text.GetLength(), &m_TextRc, DT_CENTER | DT_VCENTER, m_Color);

}

void UICheckBox::SetTexture(CString szBoxImg, CString szCheckImg)
{
	m_pBoxTex = Assets::GetTexture(szBoxImg, &m_ImageInfo);
	m_pCheckTex = Assets::GetTexture(szCheckImg, &m_ImageInfo);

	m_Size.x = m_ImageInfo.Width;
	m_Size.y = m_ImageInfo.Height;

	SetRect(&m_BoxRc, 0, 0, m_Size.x, m_Size.y);
}

void UICheckBox::SetText(LPD3DXFONT font, CString szText)
{
	m_pFont = font;
	m_Text = szText;

	D3DXFONT_DESCW fontDesc;
	m_pFont->GetDescW(&fontDesc);
	Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;

	SetRect(&m_TextRc, pos.x + m_Size.x + 5, pos.y, pos.x + m_Size.x + 5 + (fontDesc.Width * m_Text.GetLength()), pos.y + max(fontDesc.Height, m_Size.y));
}