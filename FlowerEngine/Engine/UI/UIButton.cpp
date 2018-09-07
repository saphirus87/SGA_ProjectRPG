#include "stdafx.h"
#include "UIButton.h"


UIButton::UIButton(UIButtonDelegate* pDelegate, CString buttonName)
	: UIControl(),
	m_ButtonState(eButtonState_Normal),
	m_pDelegate(pDelegate),
	m_szButtonName(buttonName),
	m_PressTimer(0.0f)
{
	m_Size = Vector2(0, 0);
}


UIButton::~UIButton()
{
}

void UIButton::Awake()
{
}

void UIButton::Update()
{
	if (!m_IsVisible) return;

	RECT rc;
	// parent Pos = ComDialog의 위치 + 자신의 위치
	Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;
	SetRect(&rc, pos.x, pos.y, pos.x + (m_Size.x * m_Scale.x), pos.y + (m_Size.y * m_Scale.y));

	POINT mousePos;
	mousePos.x = Input::GetMousePosition().x;
	mousePos.y = Input::GetMousePosition().y;

	bool IsOnMouse = PtInRect(&rc, mousePos);

	if (IsOnMouse)
	{
		if (Input::ButtonPress(VK_LBUTTON))
		{
			if (m_ButtonState == eButtonState_Mouseover)
				m_ButtonState = eButtonState_Click;
			else
				m_PressTimer += Time::Get()->GetDeltaTime();

			if (m_PressTimer > 1.0f && m_ButtonState != eButtonState_Press)
			{
				m_ButtonState = eButtonState_Press;
				if (m_pDelegate != NULL) m_pDelegate->OnPress(this);
			}
		}
		else
		{
			if (m_ButtonState == eButtonState_Click)
				if (m_pDelegate != NULL) m_pDelegate->OnClick(this);

			m_ButtonState = eButtonState_Mouseover;
			m_PressTimer = 0.0f;
		}
	}
	else
	{
		if (Input::ButtonPress(VK_LBUTTON));
		else
		{
			m_ButtonState = eButtonState_Normal;
			m_PressTimer = 0.0f;
		}
	}

}

void UIButton::Render()
{
	if (!m_IsVisible) return;

	RECT rc;
	SetRect(&rc, 0, 0, m_Size.x, m_Size.y);
	Vector3 pos = m_pParent->gameObject->transform->GetPosition() + m_Position;
	D3DXMATRIXA16 matS, matT;
	D3DXMatrixScaling(&matS, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);

	m_pSprite->SetTransform(&(matS * matT));
	m_pSprite->Draw(m_Textures[m_ButtonState], &rc, &m_Pivot, &Vector3(0, 0, 0), m_Color);
}

void UIButton::SetTexture(CString szNormalImg, CString szMouseoverImg, CString szClickImg)
{
	bool bHaveImg = true;

	if (szNormalImg != "None") 
		m_Textures[eButtonState_Normal] = Assets::GetTexture(szNormalImg, &m_ImageInfo);
	else 
	{
		m_Textures[eButtonState_Normal] = NULL;
		bHaveImg = false;
	}

	if (szMouseoverImg != "None")
		m_Textures[eButtonState_Mouseover] = Assets::GetTexture(szMouseoverImg, &m_ImageInfo);
	else 
	{
		m_Textures[eButtonState_Mouseover] = NULL;
		bHaveImg = false;
	}

	if (szClickImg != "None") 
		m_Textures[eButtonState_Click] = Assets::GetTexture(szClickImg, &m_ImageInfo);
	else 
	{
		m_Textures[eButtonState_Click] = NULL; 
		bHaveImg = false;
	}

	m_Textures[eButtonState_Press] = m_Textures[eButtonState_Click];

	if (bHaveImg)
	{
		m_Size.x = m_ImageInfo.Width;
		m_Size.y = m_ImageInfo.Height;
	}
}
