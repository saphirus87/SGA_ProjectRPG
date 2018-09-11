#include "stdafx.h"
#include "ComUIItemInfo.h"
#include "ItemInfo.h"
#include "ComChrEquipment.h"


ComUIItemInfo::ComUIItemInfo(CString szName)
	: Component(szName), m_pItemInfo(nullptr)
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
	m_pDialog->AddButton(eItemInfoUI_CloseBtn, "Resources/ui/ui-panel-minimizebutton-up.png", "Resources/ui/ui-panel-minimizebutton-up.png",
		"Resources/ui/ui-panel-minimizebutton-down.png", this, "ItemInfoClose");
	// 아이템 장착 or 사용 버튼 & 텍스트
	m_pDialog->AddButton(eItemInfoUI_UseItemBtn, "Resources/ui/UseItemBtn-up.png", "Resources/ui/UseItemBtn-up.png",
		"Resources/ui/UseItemBtn-down.png", this, "UseItem");
	m_pDialog->AddText(eItemInfoUI_UseItemText, Assets::GetFont(Assets::FontType_UseItem), "장 착");
	m_pDialog->GetText(eItemInfoUI_UseItemText)->SetDrawFormat(DT_CENTER | DT_VCENTER);

	// 아이템 정보창 업데이트
	UpdateDialog(Vector2(256.0f, 256.0f));
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
	else if (pSender->GetButtonName() == "UseItem")
	{
		ComChrEquipment* pEquipment;

		if (m_pItemInfo->ChrType == eChrType_Human)
		{
			pEquipment = (ComChrEquipment*)GameObject::Find("human_01")->GetComponent("ComChrEquipment");
		}
		else if (m_pItemInfo->ChrType == eChrType_Undead)
		{
			pEquipment = (ComChrEquipment*)GameObject::Find("undead_01")->GetComponent("ComChrEquipment");
		}
		else if (m_pItemInfo->ChrType == eChrType_Troll)
		{
			pEquipment = (ComChrEquipment*)GameObject::Find("troll_01")->GetComponent("ComChrEquipment");
		}

		pEquipment->Equip(m_pItemInfo);

		m_pItemInfo = nullptr;
		m_pDialog->SetIsVisible(false);
	}
}

void ComUIItemInfo::OnPress(UIButton * pSender)
{
}

void ComUIItemInfo::UpdateDialog(Vector2 BorderSize)
{
	if (BorderSize.x < 256.0f)
		BorderSize.x = 256.0f;
	if (BorderSize.y < 256.0f)
		BorderSize.y = 256.0f;

	// 아이템 정보창 테두리 위치 및 크기 설정
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

	// 아이템 정보창 바탕 이미지 위치 및 크기 설정
	//m_pDialog->GetImage(eItemInfoUI_Border)->SetScale(Vector3(BorderSize.x / 256.0f, BorderSize.y / 256.0f, 0.0f));
	m_pDialog->GetImage(eItemInfoUI_Border)->SetScale(Vector3(BorderSize.x / 256.0f * 1.1f, BorderSize.y / 256.0f * 1.1f, 0.0f));
	m_pDialog->GetImage(eItemInfoUI_Border)->SetPosition(Vector3(-BorderSize.x * 0.05f, -BorderSize.y * 0.05f, 0.0f));

	// 아이템 정보창 닫기 버튼 위치 설정
	m_pDialog->GetButton(eItemInfoUI_CloseBtn)->SetPosition(Vector3(BorderSize.x - 32.0f, 10.0f, 0.0f));

	// 아이템 사용 버튼 위치 및 크기 설정
	Vector2 btnSize = m_pDialog->GetButton(eItemInfoUI_UseItemBtn)->GetOriginalSize();
	m_pDialog->GetButton(eItemInfoUI_UseItemBtn)->SetPosition(Vector3(0.0f, BorderSize.y - 60.0f, 0.0f));
	m_pDialog->GetButton(eItemInfoUI_UseItemBtn)->SetScale(Vector3(BorderSize.x / btnSize.x, 1.2f, 0.0f));
	// 아이템 사용 버튼 텍스트 변경 및 위치 설정
	//m_pDialog->GetText(eItemInfoUI_UseItemText)->SetPosition()

	m_pDialog->GetText(eItemInfoUI_UseItemText)->SetPosition(m_pDialog->GetButton(eItemInfoUI_UseItemBtn)->GetPosition());
	m_pDialog->GetText(eItemInfoUI_UseItemText)->SetSize(m_pDialog->GetButton(eItemInfoUI_UseItemBtn)->GetSize());

	if (nullptr == m_pItemInfo) return;

	if (m_pItemInfo->Type == eItem_Potion)
		m_pDialog->GetText(eItemInfoUI_UseItemText)->SetText("사 용");
	else
		m_pDialog->GetText(eItemInfoUI_UseItemText)->SetText("장 착");
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

	UpdateDialog(BorderSize);
}