#pragma once
#include "stdafx.h"

class ItemInfo;
class ComCharacter;
class StatusInfo;

class FactoryGameObject
{
public:
	FactoryGameObject();
	~FactoryGameObject();

	GameObject* CreateGameObject(CString szName);
	GameObject* CreateGrid(CString szName);
	GameObject* CreateCubePN(CString szName);
	GameObject* CreateSphere(CString szName, float fPosX, float fPosY, float fPosZ);
	GameObject* CreateSunLight(float fAmbient = 0.1f, float fDiffuse = 0.2f);
	GameObject* CreatePointLight(Color* color, float fPosX = 0.0f, float fPosY = 0.0f, float fPosZ = 0.0f);
	GameObject* CreateAircraft(CString szName, int iLevel, float fPosX = 0.0f, float fPosY = 0.0f, float fPosZ = 0.0f);
	GameObject* CreateAircraftEnermy(CString szName, float fPosX = 0.0f, float fPosY = 0.0f, float fPosZ = 0.0f);
	GameObject* CreateMissile(CString szName, GameObject* who, Vector3 vDir, float fSpeed, float fRange);
	GameObject* CreateUIText(GameObject* pParent, CString szName, CString szText, float fPosX = 0.0f, float fPosY = 0.0f, float fSizeX = 100.0f, float fSizeY = 50.0f, Assets::FontType type = Assets::FontType_NORMAL);
	GameObject* CreateUIImage(GameObject* pParent, CString szName, CString szFileName, float fPosX = 0.0f, float fPosY = 0.0f);
	GameObject* CreateUIButton(GameObject* pParent, CString szName, CString szFileName, float fPosX = 0.0f, float fPosY = 0.0f);
	GameObject* CreateFromXFile(CString szName, CString szFolderPath, CString szFileName, Vector3& pos);
	GameObject* CreateObjMap(CString szName, CString szFolderPath, CString szFileName);
	GameObject* CreateEquipment(ItemInfo* pItemInfo, Vector3& pos, bool IsMirrored = false);
	GameObject* CreateEquipmentToMap(ItemInfo* pItemInfo, Vector3& pos, Vector3& mapPos, bool IsMirrored = false);
	GameObject* CreateCharacter(CString szName, CString szFolderPath, CString szFileName, CString szChrName, Vector3& pos, ComCharacter* pComChr);
	GameObject* CreateMonster(CString szName, CString szFolderPath, CString szFileName, Vector3& pos, Component* pComAI, StatusInfo* status);
	GameObject* CreateUIDialog(CString szName, Vector3& pos);
};

