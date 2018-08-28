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
		D3DXMATRIXA16 matS, matT;
		D3DXMatrixScaling(&matS, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);

		m_pSprite->SetTransform(&(matS * matT));
		m_pSprite->Draw(pTexture, &rc, &m_Pivot, &Vector3(0, 0, 0), m_Color);
	}
}

void UIImage::SetTexture(CString szFileName)
{
	if (szFileName == "None")
	{
		pTexture = NULL;

		m_Size.x = 0.0f;
		m_Size.y = 0.0f;
	}
	else
	{
		pTexture = Assets::GetTexture(szFileName, &m_imgInfo);

		m_Size.x = m_imgInfo.Width;
		m_Size.y = m_imgInfo.Height;
	}
}
