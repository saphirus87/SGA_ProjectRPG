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
	if (!m_IsVisible) return;

	RECT rc;
	SetRect(&rc, 0, 0, m_Size.x, m_Size.y);
	Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;
	D3DXMATRIXA16 matS, matT;
	D3DXMatrixScaling(&matS, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);

	m_pSprite->SetTransform(&(matS * matT));
	m_pFont->DrawTextW(m_pSprite, m_Text, m_Text.GetLength(), &rc, m_drawTextFormat, m_Color);
}

void UIText::SetText(LPD3DXFONT font, CString szText)
{
	m_pFont = font;
	m_Text = szText;

	D3DXFONT_DESCW fontDesc;

	m_pFont->GetDescW(&fontDesc);
	SetSize(Vector2(fontDesc.Width * m_Text.GetLength() * 2, fontDesc.Height));
}

//void UIText::SetText(CString szText)
//{
//	m_Text = szText;
//
//	D3DXFONT_DESCW fontDesc;
//
//	m_pFont->GetDescW(&fontDesc);
//	SetSize(Vector2(fontDesc.Width * m_Text.GetLength() * 2, fontDesc.Height));
//}

void UIText::SetFont(LPD3DXFONT font)
{
	m_pFont = font;

	D3DXFONT_DESCW fontDesc;

	m_pFont->GetDescW(&fontDesc);
	SetSize(Vector2(fontDesc.Width * m_Text.GetLength() * 2, fontDesc.Height));
}
