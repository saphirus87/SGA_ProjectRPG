#include "stdafx.h"
#include "UIProgressBar.h"

UIProgressBar::UIProgressBar()
	: UIControl(),
	m_CurValue(0),
	m_MaxValue(0)
{
}


UIProgressBar::~UIProgressBar()
{

}

void UIProgressBar::Awake()
{

}

void UIProgressBar::Update()
{
	if (Input::KeyDown(VK_F1))
		m_CurValue -= 10;


}

void UIProgressBar::Render()
{
	if (!m_IsVisible) return;

	if (pTexture)
	{
		float gaugePercent = ((float)m_CurValue / (float)m_MaxValue);
		RECT rc;
		SetRect(&rc, 0, 0, m_Size.x * gaugePercent, m_Size.y);
		Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;
		D3DXMATRIXA16 matS, matT;
		D3DXMatrixScaling(&matS, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);

		m_pSprite->SetTransform(&(matS * matT));
		m_pSprite->Draw(pTexture, &rc, &m_Pivot, &Vector3(0, 0, 0), gaugePercent * m_MaxColor + (1 - gaugePercent) * m_MinColor);
	}
}

void UIProgressBar::SetTexture(CString szFileName)
{
	pTexture = Assets::GetTexture(szFileName, &m_imgInfo);

	m_Size.x = m_imgInfo.Width;
	m_Size.y = m_imgInfo.Height;
}
