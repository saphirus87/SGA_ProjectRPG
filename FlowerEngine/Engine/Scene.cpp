#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"

Scene::Scene(CString szName) : 
	m_szName(szName)
{
}

Scene::~Scene()
{
	CleanUpGameObject();
}

void Scene::Update()
{

	for (IterGO iter = GameObject::m_mapGameObjects.begin(); iter != GameObject::m_mapGameObjects.end(); ++iter)
	{
		GameObject* pGO = (*iter).second;
		if (pGO)
		{
			// 파괴 예약된 개체면 파괴
			if (pGO->IsDestroy() == true)
			{
				GameObject::Destroy(pGO);
				GameObject::m_mapGameObjects.erase(iter);
				break;
			}
			// 그렇지 않으면 업데이트
			else
			{
				// 절두체 안에 있는 오브젝트만 업데이트
				//if (Camera::GetInstance()->FrustumCulling(&pGO->transform->GetPosition()))
				pGO->Update();
			}
		}
	}
}

bool CompareZ(const GameObject* pGO1, const GameObject* pGO2)
{
	return pGO1->transform->GetPosition().z > pGO2->transform->GetPosition().z;
}

bool CompareDist(const GameObject* pGO1, const GameObject* pGO2)
{
	return pGO1->fDistanceToCamera > pGO2->fDistanceToCamera;
}

void Scene::Render()
{
	int iRenderObjCnt = 0;

	// UI나 파티클 때문에 Z소팅
	m_listRender.clear();
	m_listRenderUI.clear();

	GameObject* pGOPostEffect = NULL;
	for (auto & go : GameObject::m_mapGameObjects)
	{
		if (go.second->Name().Find(L"UI") >= 0)
			m_listRenderUI.push_back(go.second);
		else
		{
			// 절두체 안에 있는 오브젝트들만
			if (Camera::GetInstance()->FrustumCulling(&go.second->transform->GetPosition()) ||
				go.second->IsAlwaysRender)
			{
				Vector3 camPos = Camera::GetInstance()->GetPosition();
				go.second->fDistanceToCamera = ComTransform::Distance(go.second, &camPos);

				if (go.second->Name().Find(L"PostEffect") >= 0)
					pGOPostEffect = go.second;
				else
					m_listRender.push_back(go.second);
			}
		}
	}

	m_listRender.sort(CompareDist);
	m_listRender.push_back(pGOPostEffect);
	m_listRenderUI.sort(CompareZ);

	for (auto & go : m_listRender)
	{
		// 절두체 안에 있는 게임오브젝트만 컬링
		if (go != NULL)
		{
			go->Render();
			++iRenderObjCnt;
		}
	}

	// UI Rendering
	for (auto & go : m_listRenderUI)
	{
		go->Render();
		++iRenderObjCnt;
	}

	CString szDebug;
	szDebug.Format(L"GameObject Count : %d, Render Count : %d", GameObject::m_mapGameObjects.size(), iRenderObjCnt);
	Debug::Get()->EndLine();
	Debug::Get()->AddText(szDebug);
}

void Scene::CleanUpGameObject()
{
	GameObject::CleanUpGameObject();
}