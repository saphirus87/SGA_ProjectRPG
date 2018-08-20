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
	if (m_pTexture)
	{
		float gaugePercent = ((float)m_CurValue / (float)m_MaxValue);
		RECT rc;
		SetRect(&rc, 0, 0, m_Size.x * gaugePercent, m_Size.y);
		Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;
		
		m_pSprite->Draw(m_pTexture, &rc, &m_Pivot, &pos, gaugePercent * m_MaxColor + (1 - gaugePercent) * m_MinColor);
	}
}

void UIProgressBar::SetTexture(CString szFileName)
{
	m_pTexture = Assets::GetTexture(szFileName, &m_imgInfo);

	m_Size.x = m_imgInfo.Width;
	m_Size.y = m_imgInfo.Height;
}
