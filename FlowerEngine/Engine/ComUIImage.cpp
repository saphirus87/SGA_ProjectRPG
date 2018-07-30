#include "stdafx.h"
#include "ComUIImage.h"


ComUIImage::ComUIImage(CString szName) : 
	ComUI(szName),
	m_bMirrored(false)
{
}


ComUIImage::~ComUIImage()
{
	SAFE_RELEASE(m_pTex);
}

void ComUIImage::Awake()
{
}

void ComUIImage::Update()
{
}

void ComUIImage::Render()
{
	SetRect(&m_screenRect, 0, 0, size.x, size.y);

	if (m_pTex)
	{
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		m_pSprite->SetTransform(&gameObject->transform->GetWorldMatrix());
		//m_pSprite->Draw(m_pTex, &m_rect, &m_pivot, &m_pOwner->transform->GetPosition(), m_color);
		m_pSprite->Draw(m_pTex, &m_screenRect, &m_pivot, &D3DXVECTOR3(0, 0, 0), color);
		m_pSprite->End();
	}

	DrawBorder();
}

void ComUIImage::SetTexture(CString szFileName)
{
	m_pTex = Assets::GetTexture(szFileName, &m_imgInfo);

	size.x = m_imgInfo.Width;
	size.y = m_imgInfo.Height;
}

void ComUIImage::SetFullScreen()
{
	GetClientRect(GetForegroundWindow(), &m_screenRect);

	// x는 구하고자 하는 배율
	// 이미지 크기 * x = 해상도.width
	// x = 해상도.width / 이미지 크기

	float fX = m_screenRect.right / (float)m_imgInfo.Width;
	float fY = m_screenRect.bottom / (float)m_imgInfo.Height;

	gameObject->transform->SetScale(fX, fY, 1.0f);
}