#include "stdafx.h"
#include "ComTestPicking.h"
#include "../Engine/ComMeshSphere.h"

ComTestPicking::ComTestPicking(CString szName) : 
	Component(szName)
{
}


ComTestPicking::~ComTestPicking()
{
}

void ComTestPicking::Awake()
{
}

void ComTestPicking::Update()
{
	if (Input::ButtonDown(VK_LBUTTON))
	{
		Mouse* pMouse = Input::GetInstance()->m_pMouse;
		Vector3 mousePos = Input::GetInstance()->m_pMouse->GetPosition();
		
		list<GameObject*> listSpheres = GameObject::FindAll("Sphere");

		for (auto & p : listSpheres)
		{
			ComMeshSphere* pComMeshSphere = (ComMeshSphere*)p->GetComponent("ComMeshSphere");

			Ray ray = Ray::RayAtWorldSpace(mousePos.x, mousePos.y);
			bool picked = ray.CalcIntersectSphere(&pComMeshSphere->m_boundingSphere);

			if (picked == true)
			{
				pComMeshSphere->m_material = DXUtil::RED_MTRL;
			}
			else
				pComMeshSphere->m_material = DXUtil::WHITE_MTRL;
		}
	}

	if (Input::ButtonDown(VK_RBUTTON))
	{
		Vector3 mousePos = Input::GetInstance()->m_pMouse->GetPosition();

		list<GameObject*> listSpheres = GameObject::FindAll("Sphere");

		// ���� ����� �Ÿ��� ������Ʈ�� �����Ͽ� BLUE_MTRL�� �������ش�.
		float fNearDistance = 0.f;
		float fMostNearDistance = 0.f;
		ComMeshSphere* pMostNearSphere = NULL;

		for (auto & p : listSpheres)
		{
			ComMeshSphere* pComMeshSphere = (ComMeshSphere*)p->GetComponent("ComMeshSphere");

			Ray ray = Ray::RayAtWorldSpace(mousePos.x, mousePos.y);
			bool picked = ray.CalcIntersectSphere(&pComMeshSphere->m_boundingSphere, &fNearDistance);

			if (picked == true)
			{
				// fMostNearDistance == 0�� ù�� ° ���� �ɶ�
				if (fMostNearDistance == 0 || fNearDistance <= fMostNearDistance)
				{
					fMostNearDistance = fNearDistance;
					pMostNearSphere = pComMeshSphere;
				}
				// �ߺ� üũ ����
				pComMeshSphere->m_material = DXUtil::WHITE_MTRL;
			}
			else
				pComMeshSphere->m_material = DXUtil::WHITE_MTRL;
		}

		if (pMostNearSphere)
			pMostNearSphere->m_material = DXUtil::BLUE_MTRL;
	}
}

void ComTestPicking::Render()
{
}
