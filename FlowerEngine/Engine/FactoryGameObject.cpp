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
#include "../Application/ComTerrain.h"
#include "../Application/ComEquipment.h"
#include "../Application/ComChrEquipment.h"
#include "../Application/ItemInfo.h"
#include "../Application/ComCharacter.h"
#include "../Application/ComChrControl.h"
#include "../Application/ComSmallderonAI.h"

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
	pGO->IsAlwaysRender = true;
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
		pGO->IsAlwaysRender = true;
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
		pGOFront->IsAlwaysRender = true;
		pGOFront->SetParent(pGO);
		pGOFront->transform->SetPosition(0, 0, 1.5f);
		pGOFront->transform->SetScale(0.5f, 0.5f, 0.5f);
	}
		break;

	case 3:
	{
		GameObject* pGOWingLeft = CreateCubePN(szName + "WingLeft");
		pGOWingLeft->IsAlwaysRender = true;
		pGOWingLeft->SetParent(pGO);
		pGOWingLeft->transform->SetPosition(-2.5f, 0, 0);
		pGOWingLeft->transform->SetScale(1.5f, 0.2f, 0.5f);

		GameObject* pGOWingRight = CreateCubePN(szName + "WingRight");
		pGOWingRight->IsAlwaysRender = true;
		pGOWingRight->SetParent(pGO);
		pGOWingRight->transform->SetPosition(2.5f, 0, 0);
		pGOWingRight->transform->SetScale(1.5f, 0.2f, 0.5f);
	}
		break;

	case 4:
	{
		GameObject* pGOWingTail = CreateCubePN(szName + "WingTail");
		pGOWingTail->IsAlwaysRender = true;
		pGOWingTail->SetParent(pGO);
		pGOWingTail->transform->SetPosition(0, 0, -1.5f);
		pGOWingTail->transform->SetScale(0.5f, 0.5f, 0.5f);

		GameObject* pParticle = CreateGameObject("Particle");
		pParticle->IsAlwaysRender = true;
		pParticle->AddComponent(new ComParticle("ComParticle"));
		pParticle->SetParent(pGOWingTail);
		pParticle->transform->SetPosition(0, 0, -1.5f);

		GameObject* pGOWingTailLeft = CreateCubePN(szName + "WingTailLeft");
		pGOWingTailLeft->IsAlwaysRender = true;
		pGOWingTailLeft->SetParent(pGOWingTail);
		pGOWingTailLeft->transform->SetPosition(-1.5f, 0, 0);
		pGOWingTailLeft->transform->SetScale(0.5f, 0.2f, 0.5f);

		GameObject* pGOWingTailRight = CreateCubePN(szName + "WingTailRight");
		pGOWingTailRight->IsAlwaysRender = true;
		pGOWingTailRight->SetParent(pGOWingTail);
		pGOWingTailRight->transform->SetPosition(1.5f, 0, 0);
		pGOWingTailRight->transform->SetScale(0.5f, 0.2f, 0.5f);

		GameObject* pGOWingTailTop = CreateCubePN(szName + "WingTailTop");
		pGOWingTailTop->IsAlwaysRender = true;
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
	pComEnermy->pTarget = GameObject::Find("Aircraft");
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
	// PROTOTYPE PATTERN 이미 있는 오브젝트 검사
	GameObject* pGOExist = GameObject::Find(szName);

	GameObject* pGO = new GameObject(szName);
	pGO->transform->SetPosition(pos);
	ComRenderSkinnedMesh* pComSkinnedMesh = new ComRenderSkinnedMesh("ComRenderSkinnedMesh");
	pGO->AddComponent(pComSkinnedMesh);
	
	// 이미 존재하는 게임 오브젝트라면 복제(Clone) 하여 메쉬를 공유하여 사용합니다.
	/*if (pGOExist != NULL)
	{
		ComRenderSkinnedMesh* pExist = (ComRenderSkinnedMesh*)pGOExist->GetComponent("ComRenderSkinnedMesh");
		pComSkinnedMesh->Clone(pExist);
	}
	else*/
		pComSkinnedMesh->Load(szFolderPath, szFileName);

	return pGO;
}

GameObject * FactoryGameObject::CreateObjMap(CString szName, CString szFolderPath, CString szFileName)
{
	GameObject* pGO = new GameObject(szName);
	pGO->IsAlwaysRender = true;

	ComObjMap* pComObjMap = new ComObjMap("ComObjMap");
	//ComTerrain* pComObjMap = new ComTerrain("ComTerrain");

	pComObjMap->SetFilename(szFolderPath, szFileName);
	pGO->AddComponent(pComObjMap);

	return pGO;
}

GameObject * FactoryGameObject::CreateEquipment(ItemInfo * pItemInfo, Vector3 & pos, bool IsMirrored)
{
	// PROTOTYPE PATTERN 이미 있는 오브젝트 검사
	GameObject* pGOExist = GameObject::Find(pItemInfo->Name);

	GameObject* pGOEquipment = new GameObject(pItemInfo->Name);

	ComEquipment* pEquipment = new ComEquipment("ComEquipment");
	pEquipment->IsMirrored = IsMirrored;
	pEquipment->pItemInfo = pItemInfo;
	++pItemInfo->Reference;

	// 이미 존재하는 게임 오브젝트라면 복제(Clone) 하여 메쉬를 공유하여 사용합니다.
	if (pGOExist == NULL)
	{
		pEquipment->Load(pItemInfo->FolderPath, pItemInfo->XFileName);
	}
	else
		pEquipment->Clone((ComEquipment*)pGOExist->GetComponent("ComEquipment"));

	// 변경된 텍스쳐 있을 경우 적용
	if (pItemInfo->TextureName.IsEmpty() == false)
		pEquipment->ChangeTexture(0, pItemInfo->TextureName);

	// 크기를 100으로 맞춰주는 이유는 .X File Export시 본 크기가 0.01인듯함.
	if (IsMirrored == true)
		pGOEquipment->transform->SetScale(1, -1, 1); // .X File Export시 Frame이 Max축으로 되어있음 [z, x, y축]

	pGOEquipment->AddComponent(pEquipment);
	pGOEquipment->transform->SetPosition(pos);

	return pGOEquipment;
}

GameObject * FactoryGameObject::CreateEquipmentToMap(ItemInfo * pItemInfo, Vector3 & pos, Vector3 & mapPos, bool IsMirrored)
{
	GameObject* pGOEquipment = CreateEquipment(pItemInfo, pos);
	pGOEquipment->Tag = eTag_Item;
	pGOEquipment->transform->SetPosition(mapPos);
	ComCollider* pCollider = new ComCollider("ComCollider");
	pGOEquipment->AddComponent(pCollider);
	pCollider->Set(Vector3(0, 0, 0), Vector3(0.1, 0.1, 0.1), false);
	return pGOEquipment;
}

GameObject * FactoryGameObject::CreateCharacter(CString szName, CString szFolderPath, CString szFileName, CString szChrName, Vector3& pos, ComCharacter* pComChr)
{
	GameObject* pGOChr = CreateFromXFile(szName, szFolderPath, szFileName, pos);
	pGOChr->Tag = eTag_Chracter;
	// 이 게임 오브젝트는 데미지 표시 가능
	ComText3D* pUIDamage = new ComText3D("ComText3D_Damage");
	pUIDamage->SetText("", Color(1, 0, 0, 1), 0.4f);
	pUIDamage->fOffsetPosY = 1.5f;
	pGOChr->AddComponent(pUIDamage);
	// 이 게임 오브젝트는 장비 장착 가능
	pGOChr->AddComponent(new ComChrEquipment("ComChrEquipment"));
	// 이 게임 오브젝트는 대상을 따라다님
	pGOChr->AddComponent(new ComFollowTarget("ComFollowTarget"));
	// 이 게임 오브젝트의 직업
	pGOChr->AddComponent(pComChr);
	// 이 게임 오브젝트는 컨트롤 가능
	pGOChr->AddComponent(new ComChrControl("ComChrControl"));
	// 이 게임 오브젝트는 충돌체크 가능
	ComCollider* pCollider = new ComCollider("ComCollider");
	pGOChr->AddComponent(pCollider);
	pCollider->Set(Vector3(0, 0.5f, 0), Vector3(0.3, 0.6, 0.3), false);
	// 이 게임 오브젝트는 뭔가 발사 가능(Skill사용시 필요한 구성요소)
	pGOChr->AddComponent(new ComShooting("ComShooting"));
	// 이 게임 오브젝트는 이름추가 가능
	ComText3D* pChrName = new ComText3D("ComText3D");
	pChrName->SetText(szChrName);
	pChrName->SetChrNamePos(pos);
	pGOChr->AddComponent(pChrName);
	
	return pGOChr;
}

GameObject * FactoryGameObject::CreateMonster(CString szName, CString szFolderPath, CString szFileName, Vector3 & pos, Component* pComAI, StatusInfo& status)
{
	// 몬스터 생성 (smallderon_orange)
	GameObject* pGOMonX = CreateFromXFile(szName, szFolderPath, szFileName, pos);
	pGOMonX->Tag = eTag_Monster;
	
	ComFollowTarget* pComTarget = new ComFollowTarget("ComFollowTarget");
	pComTarget->fRange = 10.0f;
	pGOMonX->AddComponent(pComTarget);
	
	// 이 게임 오브젝트는 데미지 표시 가능
	ComText3D* pUIDamage = new ComText3D("ComText3D_Damage");
	pUIDamage->SetText("", Color(1, 0, 0, 1), 0.4f);
	pUIDamage->fOffsetPosY = 2.2f;
	pGOMonX->AddComponent(pUIDamage);
	
	ComCharacter* pChrMon = (ComCharacter*)pComAI;
	pChrMon->Status = status;
	pGOMonX->AddComponent(pChrMon);

	//pGOMonX->AddComponent(pComAI);

	// pComAI 다음에
	ComRenderSkinnedMesh* pRenderSkinnedMesh = (ComRenderSkinnedMesh*)pGOMonX->GetComponent("ComRenderSkinnedMesh");
	pRenderSkinnedMesh->pCallbackHandler = new AttackHandler();
	
	// 이 게임 오브젝트는 충돌체크 가능
	ComCollider* pCollider = new ComCollider("ComCollider");
	pGOMonX->AddComponent(pCollider);
	pCollider->Set(Vector3(0, 0.7, 0), Vector3(0.5, 0.8, 0.5), false);

	// 이 게임 오브젝트는 이름추가 가능
	ComText3D* pChrName = new ComText3D("ComText3D");
	pChrName->SetText(szName);
	pChrName->fOffsetPosY = 2.0f;
	pGOMonX->AddComponent(pChrName);

	return pGOMonX;
}

GameObject * FactoryGameObject::CreateUIDialog(CString szName, Vector3& pos)
{
	GameObject* pGOUIDialog = CreateGameObject(szName);
	pGOUIDialog->transform->SetPosition(pos.x, pos.y, 0.0f);
	pGOUIDialog->fDistanceToCamera = pos.z;
	pGOUIDialog->IsAlwaysRender = true;

	ComDialog* pComDialog = new ComDialog("ComDialog");
	pGOUIDialog->AddComponent(pComDialog);

	return pGOUIDialog;
}
