#include "stdafx.h"
#include "ScenePicking.h"
#include "ComTestPicking.h"
#include "../Engine/ComLight.h"

ScenePicking::ScenePicking(CString szName) : 
	Scene(szName)
{
}


ScenePicking::~ScenePicking()
{
}

void ScenePicking::Init()
{
	FactoryGameObject factory;

	const int Count = 8;
	for (int i = -Count; i <= Count; i += 4)
		for (int j = -Count; j <= Count; j += 4)
			for (int z = -Count; z <= Count; z += 4)
				factory.CreateSphere("Sphere", i, j, z);

	GameObject* pTestPicking = new GameObject("TestPicking");
	pTestPicking->AddComponent(new ComTestPicking("ComTestPicking"));

	GameObject* pLight = GameObject::Find("Light");
	static_cast<ComLight*>(pLight->GetComponent("ComLight"))->UpdateLight(0.5f, 0.4f);
}
