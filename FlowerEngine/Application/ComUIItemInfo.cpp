#include "stdafx.h"
#include "ComUIItemInfo.h"
#include "ItemInfo.h"


ComUIItemInfo::ComUIItemInfo(CString szName)
	: Component(szName)
{
}


ComUIItemInfo::~ComUIItemInfo()
{
}

void ComUIItemInfo::Awake()
{
	m_pDialog = (ComDialog*)gameObject->GetComponent("ComDialog");

	// 아이템 정보창 바탕 이미지 생성
	m_pDialog->AddImage(eItemInfoUI_BorderTopLeft, "Resources/ui/thinborder-topleft.png");
	m_pDialog->AddImage(eItemInfoUI_BorderTop, "Resources/ui/thinborder-top.png");
	m_pDialog->AddImage(eItemInfoUI_BorderTopRight, "Resources/ui/thinborder-topright.png");
	m_pDialog->AddImage(eItemInfoUI_BorderLeft, "Resources/ui/thinborder-left.png");
	m_pDialog->AddImage(eItemInfoUI_BorderRight, "Resources/ui/thinborder-right.png");
	m_pDialog->AddImage(eItemInfoUI_BorderBottomLeft, "Resources/ui/thinborder-bottomleft.png");
	m_pDialog->AddImage(eItemInfoUI_BorderBottom, "Resources/ui/thinborder-bottom.png");
	m_pDialog->AddImage(eItemInfoUI_BorderBottomRight, "Resources/ui/thinborder-bottomright.png");
	m_pDialog->AddImage(eItemInfoUI_Border, "Resources/ui/t_vfx_border_256.png");
	m_pDialog->GetImage(eItemInfoUI_Border)->SetColor(D3DXCOLOR(0.1f, 0.1f, 0.2f, 0.5f));

	// 아이템 정보를 표시 할 텍스트 생성
	m_pDialog->AddText(eItemInfoUI_ItemNameText, Assets::GetFont(Assets::FontType_Coin), "");
	m_pDialog->GetText(eItemInfoUI_ItemNameText)->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	// 아이템 정보창 닫기 버튼
	m_pDialog->AddButton(100, "Resources/ui/ui-panel-minimizebutton-up.png", "Resources/ui/ui-panel-minimizebutton-up.png",
			"Resources/ui/ui-panel-minimizebutton-down.png", this, "ItemInfoClose");

	// 아이템 정보창 크기 수정
	SetBorderSize(Vector2(256.0f, 256.0f));
}

void ComUIItemInfo::Update()
{
}

void ComUIItemInfo::Render()
{
}

void ComUIItemInfo::OnClick(UIButton * pSender)
{
	if (pSender->GetButtonName() == "ItemInfoClose")
		m_pDialog->SetIsVisible(false);
}

void ComUIItemInfo::OnPress(UIButton * pSender)
{
}

void ComUIItemInfo::SetItemInfo(ItemInfo * pItemInfo)
{
	m_pItemInfo = pItemInfo;
	m_pDialog->GetText(eItemInfoUI_ItemNameText)->SetText(Assets::GetFont(Assets::FontType_Coin), "ItemName : " + m_pItemInfo->Name);
	m_pDialog->GetText(eItemInfoUI_ItemNameText)->SetDrawFormat(DT_LEFT | DT_VCENTER);
	m_pDialog->GetText(eItemInfoUI_ItemNameText)->SetPosition(Vector3(10.0f, 10.0f, 0.0f));

	Vector2 BorderSize;
	BorderSize.x = m_pDialog->GetText(eItemInfoUI_ItemNameText)->GetSize().x + 40.0f;
	BorderSize.y = 256.0f;

	SetBorderSize(BorderSize);
}

void ComUIItemInfo::SetBorderSize(Vector2 BorderSize)
{
	if (BorderSize.x < 256.0f)
		BorderSize.x = 256.0f;
	if (BorderSize.y < 256.0f)
		BorderSize.y = 256.0f;

	m_pDialog->GetImage(eItemInfoUI_BorderTop)->SetPosition(Vector3(32.0f, 0.0f, 0.0f));
	m_pDialog->GetImage(eItemInfoUI_BorderTop)->SetSize(Vector2(BorderSize.x - 64.0f, 32.0f));
	m_pDialog->GetImage(eItemInfoUI_BorderTopRight)->SetPosition(Vector3(BorderSize.x - 32.0f, 0.0f, 0.0f));
	m_pDialog->GetImage(eItemInfoUI_BorderLeft)->SetPosition(Vector3(0.0f, 32.0f, 0.0f));
	m_pDialog->GetImage(eItemInfoUI_BorderLeft)->SetSize(Vector2(32.0f, BorderSize.y - 64.0f));
	m_pDialog->GetImage(eItemInfoUI_BorderRight)->SetPosition(Vector3(BorderSize.x - 32.0f, 32.0f, 0.0f));
	m_pDialog->GetImage(eItemInfoUI_BorderRight)->SetSize(Vector2(32.0f, BorderSize.y - 64.0f));
	m_pDialog->GetImage(eItemInfoUI_BorderBottomLeft)->SetPosition(Vector3(0.0f, BorderSize.y - 32.0f, 0.0f));
	m_pDialog->GetImage(eItemInfoUI_BorderBottom)->SetPosition(Vector3(32.0f, BorderSize.y - 32.0f, 0.0f));
	m_pDialog->GetImage(eItemInfoUI_BorderBottom)->SetSize(Vector2(BorderSize.x - 64.0f, 32.0f));
	m_pDialog->GetImage(eItemInfoUI_BorderBottomRight)->SetPosition(Vector3(BorderSize.x - 32.0f, BorderSize.y - 32.0f, 0.0f));

	//m_pDialog->GetImage(eItemInfoUI_Border)->SetScale(Vector3(BorderSize.x / 256.0f, BorderSize.y / 256.0f, 0.0f));
	m_pDialog->GetImage(eItemInfoUI_Border)->SetScale(Vector3(BorderSize.x / 256.0f * 1.1f, BorderSize.y / 256.0f * 1.1f, 0.0f));
	m_pDialog->GetImage(eItemInfoUI_Border)->SetPosition(Vector3(-BorderSize.x * 0.05f, -BorderSize.y * 0.05f, 0.0f));

	m_pDialog->GetButton(100)->SetPosition(Vector3(BorderSize.x - 32.0f, 10.0f, 0.0f));
}
