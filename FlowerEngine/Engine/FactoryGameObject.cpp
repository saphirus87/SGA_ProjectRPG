#include "stdafx.h"
#include "FactoryGameObject.h"
#include "ComGrid.h"
#include "ComRenderCubePN.h"
#include "../Application/ComAircraftControl.h"
#include "../Application/ComAircraft.h"
#include "../Application/ComShooting.h"
#include "../Application/ComFollowTarget.h"
#include "../Application/ComAircraftEnermyAI.h"
#include "ComLight.h"
#include "../Application/ComMissile.h"
#include "ComMeshSphere.h"
#include "../Application/ComObjMap.h"

FactoryGameObject::FactoryGameObject()
{
}


FactoryGameObject::~FactoryGameObject()
{
}

GameObject * FactoryGameObject::CreateGameObject(CString szName)
{
	GameObject* pGO = new GameObject(szName);
	return pGO;
}

GameObject * FactoryGameObject::CreateGrid(CString szName)
{
	GameObject* pGO = new GameObject(szName);
	pGO->AddComponent(new ComGrid("ComGrid"));
	return pGO;
}

GameObject * FactoryGameObject::CreateCubePN(CString szName)
{
	GameObject* pGO = new GameObject(szName);
	pGO->AddComponent(new ComRenderCubePN("ComRenderCubePN"));
	return pGO;
}

GameObject * FactoryGameObject::CreateSphere(CString szName, float fPosX, float fPosY, float fPosZ)
{
	GameObject* pGO = new GameObject(szName);
	pGO->transform->SetPosition(fPosX, fPosY, fPosZ);
	pGO->AddComponent(new ComMeshSphere("ComMeshSphere"));
	return pGO;
}

GameObject* FactoryGameObject::CreateSunLight(float fAmbient, float fDiffuse)
{
	GameObject* pGO = new GameObject("Light");
	ComLight* pLight = new ComLight("ComLight");
	pLight->InitDirectional(fAmbient, fDiffuse);
	pGO->AddComponent(pLight);
	return pGO;
}

GameObject * FactoryGameObject::CreatePointLight(Color * color, float fPosX, float fPosY, float fPosZ)
{
	GameObject* pGO = new GameObject("Light");
	ComLight* pLight = new ComLight("ComLight");
	pLight->InitPoint(fPosX, fPosY, fPosZ, color);
	pGO->AddComponent(pLight);
	return pGO;
}

GameObject * FactoryGameObject::CreateAircraft(CString szName, int iLevel, float fPosX, float fPosY, float fPosZ)
{
	GameObject* pGO = GameObject::Find(szName);

	switch (iLevel)
	{
	case 1:
	{
		GameObject* pGO = CreateCubePN(szName);
		pGO->AddComponent(new ComAircraft("ComAircraft"));
		pGO->AddComponent(new ComShooting("ComShooting"));
		pGO->AddComponent(new ComAircraftControl("ComAircraftControl"));
		pGO->AddComponent(new ComCollider("ComCollider"));
		Camera::GetInstance()->SetTarget(&pGO->transform->GetPosition());
	}
		break;

	case 2:
	{
		GameObject* pGOFront = CreateCubePN(szName + "Front");
		pGOFront->SetParent(pGO);
		pGOFront->transform->SetPosition(0, 0, 1.5f);
		pGOFront->transform->SetScale(0.5f, 0.5f, 0.5f);
	}
		break;

	case 3:
	{
		GameObject* pGOWingLeft = CreateCubePN(szName + "WingLeft");
		pGOWingLeft->SetParent(pGO);
		pGOWingLeft->transform->SetPosition(-2.5f, 0, 0);
		pGOWingLeft->transform->SetScale(1.5f, 0.2f, 0.5f);

		GameObject* pGOWingRight = CreateCubePN(szName + "WingRight");
		pGOWingRight->SetParent(pGO);
		pGOWingRight->transform->SetPosition(2.5f, 0, 0);
		pGOWingRight->transform->SetScale(1.5f, 0.2f, 0.5f);
	}
		break;

	case 4:
	{
		GameObject* pGOWingTail = CreateCubePN(szName + "WingTail");
		pGOWingTail->SetParent(pGO);
		pGOWingTail->transform->SetPosition(0, 0, -1.5f);
		pGOWingTail->transform->SetScale(0.5f, 0.5f, 0.5f);

		GameObject* pParticle = CreateGameObject("Particle");
		pParticle->AddComponent(new ComParticle("ComParticle"));
		pParticle->SetParent(pGOWingTail);
		pParticle->transform->SetPosition(0, 0, -1.5f);

		GameObject* pGOWingTailLeft = CreateCubePN(szName + "WingTailLeft");
		pGOWingTailLeft->SetParent(pGOWingTail);
		pGOWingTailLeft->transform->SetPosition(-1.5f, 0, 0);
		pGOWingTailLeft->transform->SetScale(0.5f, 0.2f, 0.5f);

		GameObject* pGOWingTailRight = CreateCubePN(szName + "WingTailRight");
		pGOWingTailRight->SetParent(pGOWingTail);
		pGOWingTailRight->transform->SetPosition(1.5f, 0, 0);
		pGOWingTailRight->transform->SetScale(0.5f, 0.2f, 0.5f);

		GameObject* pGOWingTailTop = CreateCubePN(szName + "WingTailTop");
		pGOWingTailTop->SetParent(pGOWingTail);
		pGOWingTailTop->transform->SetPosition(0, 1.5f, 0);
		pGOWingTailTop->transform->SetScale(0.2f, 0.5f, 0.5f);
	}
		break;
	}

	return pGO;
}

GameObject * FactoryGameObject::CreateAircraftEnermy(CString szName, float fPosX, float fPosY, float fPosZ)
{
	GameObject* pGOEnermy = CreateCubePN(szName);
	ComFollowTarget* pComEnermy = new ComFollowTarget("ComFollowTarget");
	pGOEnermy->AddComponent(pComEnermy);
	pGOEnermy->transform->SetPosition(fPosX, fPosY, fPosZ);
	
	ComShooting* pShooting = new ComShooting("ComShooting");
	pGOEnermy->AddComponent(pShooting);

	ComAircraftEnermyAI* pAircraftEnermyAI = new ComAircraftEnermyAI("ComAircraftEnermyAI");
	pGOEnermy->AddComponent(pAircraftEnermyAI);

	ComCollider* pCollider = new ComCollider("ComCollider");
	pGOEnermy->AddComponent(pCollider);

	return pGOEnermy;
}

GameObject * FactoryGameObject::CreateMissile(CString szName, GameObject* who, Vector3 vDir, float fSpeed, float fRange)
{
	GameObject* pMissile = new GameObject(szName);
	
	pMissile->transform->SetScale(0.2f, 0.2f, 0.2f);
	pMissile->transform->SetPosition(who->transform->GetPosition());
	pMissile->transform->SetRotation(who->transform->GetRotation());

	ComCollider* pCollider = new ComCollider("ComCollider");
	pMissile->AddComponent(pCollider);

	ComMissile* comMissile = new ComMissile("ComMissile");
	comMissile->Set((ComShooting*)who->GetComponent("ComShooting"), fRange);
	comMissile->SetStartPos(who->transform->GetPosition());
	comMissile->SetDirection(vDir, fSpeed);
	pMissile->AddComponent(comMissile);

	// 설정 다 되고 렌더링 되게끔
	ComRenderCubePN* pRenderCube = new ComRenderCubePN("ComRenderCubePN");
	pRenderCube->IsRenderShader = true;
	pMissile->AddComponent(pRenderCube);

	return pMissile;
}

GameObject * FactoryGameObject::CreateUIText(GameObject* pParent, CString szName, CString szText, float fPosX, float fPosY, float fSizeX, float fSizeY, Assets::FontType type)
{
	GameObject* pGOUIText = CreateGameObject(szName);
	pGOUIText->SetParent(pParent);
	pGOUIText->IsAlwaysRender = true;
	pGOUIText->transform->SetPosition(fPosX, fPosY, 0.f);

	ComUIText* pComUIText = new ComUIText("ComUIText");
	pComUIText->SetFont(type);
	pComUIText->text = szText;
	//pComUIText->size.x = szText.GetLength() * 15.0f; // 글자 하나당 15pixel
	pComUIText->size.x = fSizeX;
	pComUIText->size.y = fSizeY;
	pGOUIText->AddComponent(pComUIText);
	return pGOUIText;
}

GameObject * FactoryGameObject::CreateUIImage(GameObject* pParent, CString szName, CString szFileName, float fPosX, float fPosY)
{
	GameObject* pGOUIImage = CreateGameObject(szName);
	pGOUIImage->transform->SetPosition(fPosX, fPosY, 0);
	pGOUIImage->SetParent(pParent);
	pGOUIImage->IsAlwaysRender = true;

	ComUIImage* pComUIText = new ComUIImage("ComUIImage");
	pComUIText->SetTexture(szFileName);
	pGOUIImage->AddComponent(pComUIText);
	return pGOUIImage;
}

GameObject * FactoryGameObject::CreateUIButton(GameObject* pParent, CString szName, CString szFileName, float fPosX, float fPosY)
{
	GameObject* pGOUIButton = CreateGameObject(szName);
	pGOUIButton->SetParent(pParent);
	pGOUIButton->IsAlwaysRender = true;
	pGOUIButton->transform->SetPosition(fPosX, fPosY, 0.f);

	ComUIImage* pComUIImage = new ComUIImage("ComUIImage");
	pComUIImage->SetTexture(szFileName);
	pGOUIButton->AddComponent(pComUIImage);

	ComUIButton* pComUIButton = new ComUIButton("ComUIButton");
	pComUIButton->size = pComUIImage->size;
	pGOUIButton->AddComponent(pComUIButton);
	return pGOUIButton;
}

GameObject * FactoryGameObject::CreateFromXFile(CString szName, CString szFolderPath, CString szFileName, Vector3 & pos)
{
	GameObject* pGOExist = GameObject::Find(szName);

	GameObject* pGO = new GameObject(szName);
	pGO->transform->SetPosition(pos);
	ComRenderSkinnedMesh* pComSkinnedMesh = new ComRenderSkinnedMesh("ComRenderSkinnedMesh");
	pGO->AddComponent(pComSkinnedMesh);
	
	// 이미 존재하는 게임 오브젝트라면 복제(Clone) 하여 메쉬를 공유하여 사용합니다.
	if (pGOExist != NULL)
	{
		ComRenderSkinnedMesh* pExist = (ComRenderSkinnedMesh*)pGOExist->GetComponent("ComRenderSkinnedMesh");
		pComSkinnedMesh->Clone(pExist);
	}
	else
		pComSkinnedMesh->Load(szFolderPath, szFileName);

	return pGO;
}

GameObject * FactoryGameObject::CreateObjMap(CString szName, CString szFolderPath, CString szFileName)
{
	GameObject* pGO = new GameObject(szName);
	ComObjMap* pComObjMap = new ComObjMap("ComObjMap");
	pComObjMap->SetFilename(szFolderPath, szFileName);
	pGO->AddComponent(pComObjMap);

	return pGO;
}

GameObject * FactoryGameObject::CreateEquipmentShoulder(CString szName, CString szFolderPath, CString szFileName, Vector3& pos, GameObject* goPrefab, bool IsMirrored)
{
	GameObject* pGOEquipment = new GameObject(szName);

	ComRenderXMesh* pMesh = new ComRenderXMesh("ComRenderXMesh");
	pMesh->IsMirrored = IsMirrored;
	if (goPrefab == NULL)
	{
		pMesh->Load(szFolderPath, szFileName);
	}
	else
	{
		pMesh->Clone((ComRenderXMesh*)goPrefab->GetComponent("ComRenderXMesh"));
	}

	if (IsMirrored == false)
		pGOEquipment->transform->SetScale(100, 100, 100); // .X File Export시 Frame이 Max축으로 되어있음 [z, x, y축]
	else
		pGOEquipment->transform->SetScale(100, -100, 100); // .X File Export시 Frame이 Max축으로 되어있음 [z, x, y축]

	pGOEquipment->AddComponent(pMesh);
	pGOEquipment->transform->SetPosition(pos);
	
	return pGOEquipment;
}
