#pragma once
#include "stdafx.h"

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
};

