#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"

SceneManager* SceneManager::m_pInstance = NULL;

SceneManager::SceneManager() : 
	m_pCurrentScene(NULL)
{
}


SceneManager::~SceneManager()
{
	IterScene iter;
	for (iter = m_mapScenes.begin(); iter != m_mapScenes.end(); ++iter)
		SAFE_DELETE((*iter).second);
}

SceneManager * SceneManager::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new SceneManager();

	return m_pInstance;
}

void SceneManager::Delete()
{
	SAFE_DELETE(m_pInstance);
}

void SceneManager::Awake()
{
	for (auto & go : GameObject::m_mapGameObjects)
		go.second->Awake();
}

void SceneManager::Update()
{
	if (m_pCurrentScene)
		m_pCurrentScene->Update();
}

void SceneManager::Render()
{
	if (m_pCurrentScene)
		m_pCurrentScene->Render();
}

void SceneManager::AddScene(Scene * pScene)
{
	if (m_pCurrentScene)
		m_pCurrentScene->CleanUpGameObject();

	m_mapScenes.insert(MScenePair(pScene->Name(), pScene));
	m_pCurrentScene = pScene;
}

void SceneManager::RemoveScene(CString szName)
{
}

void SceneManager::ChangeScene(CString szName)
{
	IterScene iter = m_mapScenes.find(szName);
	if (iter != m_mapScenes.end())
	{
		m_pCurrentScene = (*iter).second;
		
		// ¾ÀÀÌ ¹Ù²ð ¶§, ÇØ¾ß ÇÒ °Íµé
		FactoryGameObject factory;
		factory.CreateSunLight();
		Camera::GetInstance()->Init();

		(*iter).second->Init();
	}
}
