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

	// UI�� ��ƼŬ ������ Z����
	m_listRender.clear();
	m_listRenderUI.clear();

	// Render���� �� for�� ������ �ʰ� ���⿡�� ���� ó��
	for (IterGO iter = GameObject::m_mapGameObjects.begin(); iter != GameObject::m_mapGameObjects.end(); ++iter)
	{
		GameObject* pGO = (*iter).second;
		if (pGO)
		{
			// �ı� ����� ��ü�� �ı�
			if (pGO->IsDestroy() == true)
			{
				GameObject::Destroy(pGO);
				GameObject::m_mapGameObjects.erase(iter);
				break;
			}
			// �׷��� ������ ������Ʈ
			else
			{
				// ����ü �ȿ� �ִ� ������Ʈ�� ������Ʈ
				//if (Camera::GetInstance()->FrustumCulling(&pGO->transform->GetPosition()))
				pGO->Update();

				// Update�� �߱� ������ ���⿡�� ������ �ø� ���־ ��
				if (pGO->Name().Find(L"UI") >= 0)
					m_listRenderUI.push_back(pGO);
				else
				{
					// ����ü �ȿ� �ִ� ������Ʈ�鸸
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
	// ���� ���� �ʿ�
	// ��ó�� ������ �� ��
	if (m_pGOPostEffect)
		m_pGOPostEffect->Render();
	else // Scene Aircraft�� �ٸ� ���϶�
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
		// ����ü �ȿ� �ִ� ���ӿ�����Ʈ�� �ø�
		if (go != NULL)
		{
			go->Render();
			++m_iRenderObjCnt;
		}
	}
}