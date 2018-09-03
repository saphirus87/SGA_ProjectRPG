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
	GameObject* m_pUILoading = factory.CreateUIDialog("ScreenUI", 0, 0);
	ComDialog* comDialog = (ComDialog*)m_pUILoading->GetComponent("ComDialog");
	comDialog->AddImage(eUI_Loading, "./Resources/ui/loadingWide.png");
	comDialog->AddProgressBar(eUI_LoadingBar, "./Resources/ui/6.tga");

	if (m_pUILoading)
	{
		comDialog->SetIsVisible(true);
	}

	SceneManager::GetInstance()->AddScene(new SceneRPG("SceneRPG"));
	SceneManager::GetInstance()->ChangeScene("SceneRPG");
}
