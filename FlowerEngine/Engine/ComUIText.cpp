#include "stdafx.h"
#include "ComUIText.h"


ComUIText::ComUIText(CString szName) : 
	ComUI(szName),
	m_pFont(NULL),
	m_drawTextFormat(DT_LEFT | DT_VCENTER)
{
}

ComUIText::~ComUIText()
{
}

void ComUIText::Awake()
{
}

void ComUIText::Update()
{
}

void ComUIText::Render()
{
	if (m_pFont)
	{
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		m_pSprite->SetTransform(&gameObject->transform->GetWorldMatrix());

		Matrix4x4 matW = gameObject->transform->GetWorldMatrix();
		Matrix4x4 matL = gameObject->transform->GetLocalMatrix();

		// 41 = transform matrix value
		// 11 = scale matrix value
		int left = matW._41;
		int top = matW._42;
		int right = left + size.x * matL._11;
		int bottom = top + size.y * matL._22;

		SetRect(&m_screenRect, left, top, right, bottom);

		m_pFont->DrawText(NULL, text, text.GetLength(), &m_screenRect, m_drawTextFormat, color);

		m_pSprite->End();

		DrawBorder();
	}
}

void ComUIText::SetFont(Assets::FontType type)
{
	switch (type)
	{
	case Assets::FontType_NORMAL:
		m_pFont = Assets::GetFont(Assets::FontType_NORMAL);
		break;
	case Assets::FontType_QUEST:
		m_pFont = Assets::GetFont(Assets::FontType_QUEST);
		break;
	}
}
