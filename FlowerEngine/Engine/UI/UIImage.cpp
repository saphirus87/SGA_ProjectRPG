#include "stdafx.h"
#include "UIImage.h"

UIImage::UIImage()
	: UIControl()
{
}


UIImage::~UIImage()
{

}

void UIImage::Awake()
{

}

void UIImage::Update()
{

}

void UIImage::Render()
{
	if (pTexture)
	{
		RECT rc;
		SetRect(&rc, 0, 0, m_Size.x, m_Size.y);
		Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;
		
		m_pSprite->Draw(pTexture, &rc, &m_Pivot, &pos, m_Color);
	}
}

void UIImage::SetTexture(CString szFileName)
{
	pTexture = Assets::GetTexture(szFileName, &m_imgInfo);

	m_Size.x = m_imgInfo.Width;
	m_Size.y = m_imgInfo.Height;
}
