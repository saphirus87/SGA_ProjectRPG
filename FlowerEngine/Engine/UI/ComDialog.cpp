#include "stdafx.h"
#include "ComDialog.h"

ComDialog::ComDialog(CString szName)
	: Component(szName), m_pSprite(NULL)
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
	for (auto p : m_Controls)
	{
		p.second->Update();
	}
}

void ComDialog::Render()
{
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

void ComDialog::AddButton(UINT id, CString szNormalImg, CString szMouseoverImg, CString szClickImg)
{
	UIButton* button = new UIButton;
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