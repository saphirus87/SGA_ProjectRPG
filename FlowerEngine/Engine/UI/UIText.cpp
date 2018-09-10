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

	// 폰트의 크기를 얻어와서 글자수에 맞게 크기 지정
	m_pFont->GetDescW(&fontDesc);
	// 한글의 경우 문자의 크기가 크기 때문에 문자열 내에 한글 갯수 만큼 크기를 늘림
	SetSize(Vector2(fontDesc.Width * (m_Text.GetLength() + GetHangulNum() + 2), fontDesc.Height));
}

void UIText::SetFont(LPD3DXFONT font)
{
	m_pFont = font;

	D3DXFONT_DESCW fontDesc;

	m_pFont->GetDescW(&fontDesc);
	SetSize(Vector2(fontDesc.Width * m_Text.GetLength() * 2, fontDesc.Height));
}

int UIText::GetHangulNum()
{
	int HangulNum = 0;

	// 텍스트 내에 한글이 몇 글자 있는지 판별
	for (int i = 0; i < m_Text.GetLength(); i++)
	{
		// i번째 문자가 한글인 조건
		if (0x80 == (m_Text.GetAt(i) & 0x80))
			++HangulNum;
	}

	return HangulNum;
}
