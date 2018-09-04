#include "stdafx.h"
#include "SceneLoading.h"
#include "SceneRPG.h"

SceneLoading::SceneLoading(CString szName) : 
	Scene(szName)
{
}


SceneLoading::~SceneLoading()
{
}

void SceneLoading::Init()
{
	FactoryGameObject factory;
	GameObject* m_pUILoading = factory.CreateUIDialog("ScreenUI", Vector3(0, 0, 0));
	ComDialog* comDialog = (ComDialog*)m_pUILoading->GetComponent("ComDialog");

	float fScreenWidth = DXUTGetWindowWidth();
	float fScreenHeight = DXUTGetWindowHeight();

	// 배경 이미지
	comDialog->AddImage(eUI_Loading, "./Resources/ui/loadingWide.png");

	// 로딩바 배경
	comDialog->AddImage(eUI_LoadingBarFrame, "./Resources/ui/loading-barborder-frame-v2.png");
	UIImage* pLoadingBarFrame = comDialog->GetImage(eUI_LoadingBarFrame);
	pLoadingBarFrame->SetPosition(Vector3(fScreenWidth / 2 - 512, fScreenHeight - 100.0f, 0));

	// 로딩바
	comDialog->AddProgressBar(eUI_LoadingBar, "./Resources/ui/loading-barfill.png");
	UIProgressBar* pLoadingBar = comDialog->GetProgressBar(eUI_LoadingBar);
	pLoadingBar->SetCurValue(0);
	pLoadingBar->SetPosition(Vector3(fScreenWidth / 2 - 482, fScreenHeight - 85.0f, 0));
	pLoadingBar->SetSize(Vector2(964.0f, 64.0f));
	pLoadingBar->SetScale(Vector3(1.0f, 0.5f, 0.0f));
	pLoadingBar->SetMaxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLoadingBar->SetMinColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 게임 시작 버튼
	comDialog->AddButton(eUI_StartBtn,
		"Resources/ui/Button-up.png",
		"Resources/ui/Button-up.png",
		"Resources/ui/Button-down.png", this, "start");
	UIButton* pBtnStart = comDialog->GetButton(eUI_StartBtn);
	pBtnStart->SetPosition(Vector3(fScreenWidth / 2 - 145, fScreenHeight - 200.0f, 0.0f));
	pBtnStart->SetScale(Vector3(0.7f, 0.5f, 0.0f));

	// 게임 시작 텍스트
	comDialog->AddText(eUI_StartText, Assets::GetFont(Assets::FontType_NORMAL), "게임 시작");
	UIText* uiTextStart = comDialog->GetText(eUI_StartText);
	uiTextStart->SetPosition(Vector3(fScreenWidth / 2 - 64, fScreenHeight - 180.0f, 0.0f));
	uiTextStart->SetDrawFormat(DT_CENTER);

	if (m_pUILoading)
		comDialog->SetIsVisible(true);
}


void SceneLoading::OnClick(UIButton * pSender)
{
	if (pSender->GetButtonName() == "start")
	{
		SceneManager::GetInstance()->AddScene(new SceneRPG("SceneRPG"), false);
		SceneManager::GetInstance()->ChangeScene("SceneRPG");
	}
}

void SceneLoading::OnPress(UIButton * pSender)
{
}
