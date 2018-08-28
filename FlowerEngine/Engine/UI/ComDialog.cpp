#include "stdafx.h"
#include "ComDialog.h"

ComDialog::ComDialog(CString szName)
	: Component(szName), m_pSprite(NULL), m_ToggleKey(0),
	m_IsVisible(false), m_Moveable(false), m_IsDrag(false)
{
}

ComDialog::~ComDialog()
{
	SAFE_RELEASE(m_pSprite);

	for (auto & o : m_Controls)
		SAFE_DELETE(o.second);
}

void ComDialog::Awake()
{
	D3DXCreateSprite(GetD3D9Device(), &m_pSprite);
}

void ComDialog::Update()
{
	if (m_ToggleKey != 0)
	{
		if (Input::KeyDown(m_ToggleKey))
			m_IsVisible = !m_IsVisible;
	}

	if (!m_IsVisible) return;

	if (m_Moveable)
	{
		if (IsOnMouse())
		{
			if (Input::ButtonDown(VK_LBUTTON)) m_IsDrag = true;
			else if ((m_IsDrag && Input::ButtonUp(VK_LBUTTON)) || Input::ButtonDoubleClick(VK_LBUTTON)) m_IsDrag = false;
			else if (m_IsDrag) gameObject->transform->SetPosition(gameObject->transform->GetPosition() + Input::m_pMouse->GetDeltaPosition());
		}
		else
		{
			m_IsDrag = false;
		}
	}


	for (auto p : m_Controls)
	{
		p.second->Update();
	}
}

void ComDialog::Render()
{
	if (!m_IsVisible) return;

	if (m_pSprite)
	{
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

		for (auto p : m_Controls)
		{
			p.second->Render();
		}

		m_pSprite->End();
	}
}

bool ComDialog::IsOnMouse()
{
	for (auto p : m_Controls)
	{
		if (p.second->IsOnMouse()) return true;
	}

	return false;
}

void ComDialog::AddImage(UINT id, CString szFileName)
{
	UIImage* img = new UIImage;
	img->SetSprite(m_pSprite);
	img->SetParent(this);
	img->SetTexture(szFileName);

	m_Controls.insert(make_pair(id, img));
}

void ComDialog::AddText(UINT id, LPD3DXFONT pFont, CString szText)
{
	UIText* text = new UIText;
	text->SetSprite(m_pSprite);
	text->SetParent(this);
	text->SetText(pFont, szText);

	m_Controls.insert(make_pair(id, text));
}

void ComDialog::AddButton(UINT id, CString szNormalImg, CString szMouseoverImg, CString szClickImg, UIButtonDelegate* pDelegate, CString szButtonName)
{
	UIButton* button = new UIButton(pDelegate, szButtonName);
	button->SetSprite(m_pSprite);
	button->SetParent(this);
	button->SetTexture(szNormalImg, szMouseoverImg, szClickImg);

	m_Controls.insert(make_pair(id, button));
}

void ComDialog::AddProgressBar(UINT id, CString szFileName)
{
	UIProgressBar* bar = new UIProgressBar;
	bar->SetSprite(m_pSprite);
	bar->SetParent(this);
	bar->SetTexture(szFileName);

	bar->SetMaxValue(100);
	bar->SetCurValue(100);
	bar->SetMaxColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	bar->SetMinColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	m_Controls.insert(make_pair(id, bar));
}

void ComDialog::AddCheckBox(UINT id, CString szBoxImg, CString szCheckImg, LPD3DXFONT pFont, CString szText)
{
	UICheckBox* box = new UICheckBox;
	box->SetSprite(m_pSprite);
	box->SetParent(this);
	box->SetTexture(szBoxImg, szCheckImg);
	box->SetText(pFont, szText);


	m_Controls.insert(make_pair(id, box));
}

void ComDialog::AddRadioButton(UINT id, CString szButtonImg, CString szCheckImg, LPD3DXFONT pFont, CString szText)
{
	UIRadioButton* radio = new UIRadioButton;
	radio->SetSprite(m_pSprite);
	radio->SetParent(this);
	radio->SetTexture(szButtonImg, szCheckImg);
	radio->SetFont(pFont);
	radio->AddRadioButton(szText);

	m_Controls.insert(make_pair(id, radio));
}
