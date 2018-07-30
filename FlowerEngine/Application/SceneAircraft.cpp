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

	// fPos�� ������ ���� ��

	factory.CreateAircraftEnermy("Enermy", fPos, 0.0f, 0.0f);	// ��
	factory.CreateAircraftEnermy("Enermy", -fPos, 0.0f, 0.0f);	// ��
	factory.CreateAircraftEnermy("Enermy", 0.0f, 0.0f, -fPos);	// ��
	factory.CreateAircraftEnermy("Enermy", 0.0f, 0.0f, fPos);	// ��

	
	factory.CreateAircraftEnermy("Enermy", fPos, 0.0f, fPos);	// ����
	factory.CreateAircraftEnermy("Enermy", fPos, 0.0f, -fPos);	// ����
	factory.CreateAircraftEnermy("Enermy", -fPos, 0.0f, fPos);	// ����
	factory.CreateAircraftEnermy("Enermy", -fPos, 0.0f, -fPos);	// ����

	factory.CreateAircraftEnermy("Enermy", fPos, 10.0f, fPos);		// ����, ��
	factory.CreateAircraftEnermy("Enermy", fPos, 10.0f, -fPos);		// ����, ��
	factory.CreateAircraftEnermy("Enermy", -fPos, -10.0f, fPos);	// ����, ��
	factory.CreateAircraftEnermy("Enermy", -fPos, -10.0f, -fPos);	// ����, ��
}
