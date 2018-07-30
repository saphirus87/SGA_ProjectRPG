#include "stdafx.h"
#include "SceneAircraft.h"
#include "ComUIPanelDialog.h"

SceneAircraft::SceneAircraft(CString szName) :
	Scene(szName)
{
}

SceneAircraft::~SceneAircraft()
{
}

void SceneAircraft::Init()
{
	FactoryGameObject factory;

	GameObject* pUIPanel = factory.CreateGameObject("UIPanelDialog");
	ComUIPanelDialog* pPanel = new ComUIPanelDialog("ComUIPanelDialog");
	pUIPanel->AddComponent(pPanel);

	factory.CreateAircraft("Aircraft", 1);

	factory.CreateGrid("Grid");
	factory.CreateSunLight();
	factory.CreatePointLight(&Color(1.0f, 0.0f, 0.0f, 1.0f), -10.0f, 10.0f, -10.0f);
	factory.CreatePointLight(&Color(1.0f, 1.0f, 0.0f, 1.0f), 10.0f, 10.0f, 10.0f);
	factory.CreatePointLight(&Color(1.0f, 0.0f, 1.0f, 1.0f), -10.0f, 10.0f, 10.0f);

	for (int i = 20; i <= 50; i += 5)
		CreateEnermy(i);
}

void SceneAircraft::CreateEnermy(float fPos)
{
	FactoryGameObject factory;

	// fPos¿¡ ·»´ý°ª ³ÖÀ» °Í

	factory.CreateAircraftEnermy("Enermy", fPos, 0.0f, 0.0f);	// µ¿
	factory.CreateAircraftEnermy("Enermy", -fPos, 0.0f, 0.0f);	// ¼­
	factory.CreateAircraftEnermy("Enermy", 0.0f, 0.0f, -fPos);	// ³²
	factory.CreateAircraftEnermy("Enermy", 0.0f, 0.0f, fPos);	// ºÏ

	
	factory.CreateAircraftEnermy("Enermy", fPos, 0.0f, fPos);	// µ¿ºÏ
	factory.CreateAircraftEnermy("Enermy", fPos, 0.0f, -fPos);	// µ¿³²
	factory.CreateAircraftEnermy("Enermy", -fPos, 0.0f, fPos);	// ¼­ºÏ
	factory.CreateAircraftEnermy("Enermy", -fPos, 0.0f, -fPos);	// ¼­³²

	factory.CreateAircraftEnermy("Enermy", fPos, 10.0f, fPos);		// µ¿ºÏ, »ó
	factory.CreateAircraftEnermy("Enermy", fPos, 10.0f, -fPos);		// µ¿³², »ó
	factory.CreateAircraftEnermy("Enermy", -fPos, -10.0f, fPos);	// ¼­ºÏ, ÇÏ
	factory.CreateAircraftEnermy("Enermy", -fPos, -10.0f, -fPos);	// ¼­³², ÇÏ
}
