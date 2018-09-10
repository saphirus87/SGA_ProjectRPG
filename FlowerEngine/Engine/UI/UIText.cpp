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

	// ��Ʈ�� ũ�⸦ ���ͼ� ���ڼ��� �°� ũ�� ����
	m_pFont->GetDescW(&fontDesc);
	// �ѱ��� ��� ������ ũ�Ⱑ ũ�� ������ ���ڿ� ���� �ѱ� ���� ��ŭ ũ�⸦ �ø�
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

	// �ؽ�Ʈ ���� �ѱ��� �� ���� �ִ��� �Ǻ�
	for (int i = 0; i < m_Text.GetLength(); i++)
	{
		// i��° ���ڰ� �ѱ��� ����
		if (0x80 == (m_Text.GetAt(i) & 0x80))
			++HangulNum;
	}

	return HangulNum;
}
