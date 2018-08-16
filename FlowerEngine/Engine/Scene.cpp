#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"

bool CompareZ(const GameObject* pGO1, const GameObject* pGO2)
{
	return pGO1->transform->GetPosition().z > pGO2->transform->GetPosition().z;
}

bool CompareDist(const GameObject* pGO1, const GameObject* pGO2)
{
	return pGO1->fDistanceToCamera > pGO2->fDistanceToCamera;
}

Scene::Scene(CString szName) : 
	m_szName(szName),
	m_pGOPostEffect(NULL),
	m_iRenderObjCnt(0)
{
}

Scene::~Scene()
{
	CleanUpGameObject();
}

void Scene::Update()
{
	m_iRenderObjCnt = 0;

	// UI나 파티클 때문에 Z소팅
	m_listRender.clear();
	m_listRenderUI.clear();

	// Render에서 또 for문 돌리지 않고 여기에서 전부 처리
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

				// Update를 했기 때문에 여기에서 렌더링 컬링 해주어도 됨
				if (pGO->Name().Find(L"UI") >= 0)
					m_listRenderUI.push_back(pGO);
				else
				{
					// 절두체 안에 있는 오브젝트들만
					if (Camera::GetInstance()->FrustumCulling(&pGO->transform->GetPosition()) ||
						pGO->IsAlwaysRender)
					{
						Vector3 camPos = Camera::GetInstance()->GetPosition();
						pGO->fDistanceToCamera = ComTransform::Distance(pGO, &camPos);

						if (pGO->Name().Find(L"PostEffect") >= 0)
							m_pGOPostEffect = pGO;
						else
							m_listRender.push_back(pGO);
					}
				}
			}
		}
	}
	
	m_listRender.sort(CompareDist);
	//m_listRender.push_back(m_pGOPostEffect);
	m_listRenderUI.sort(CompareZ);
}

void Scene::Render()
{
	// 설계 정리 필요
	// 후처리 렌더링 일 때
	if (m_pGOPostEffect)
		m_pGOPostEffect->Render();
	else // Scene Aircraft등 다른 씬일때
	{
		for (auto & go : m_listRender)
		{
			go->Render();
			++m_iRenderObjCnt;
		}
	}

	// UI Rendering
	for (auto & go : m_listRenderUI)
	{
		go->Render();
		++m_iRenderObjCnt;
	}

	CString szDebug;
	szDebug.Format(L"GameObject Count : %d, Render Count : %d", GameObject::m_mapGameObjects.size(), m_iRenderObjCnt);
	Debug::Get()->EndLine();
	Debug::Get()->AddText(szDebug);
}

void Scene::CleanUpGameObject()
{
	GameObject::CleanUpGameObject();
}

void Scene::RenderObjects()
{
	for (auto & go : m_listRender)
	{
		// 절두체 안에 있는 게임오브젝트만 컬링
		if (go != NULL)
		{
			go->Render();
			++m_iRenderObjCnt;
		}
	}
}