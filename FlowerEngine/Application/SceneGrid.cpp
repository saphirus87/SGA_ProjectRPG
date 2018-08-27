#include "stdafx.h"
#include "SceneGrid.h"
#include "../Application/ComTestCubeControl.h"
#include "../Application/ComHuman01.h"
#include "../Application/ComChrEquipment.h"
#include "../Application/ComUndead01.h"
#include "../Application/ComChrControl.h"
#include "../Application/ComTroll01.h"
#include "../Application/ComSmallderonAI.h"
#include "../Application/ItemInfo.h"
#include "../Application/ComEquipment.h"
#include "../Application/ComFollowTarget.h"

SceneGrid::SceneGrid(CString szName) : Scene(szName)
{
}

SceneGrid::~SceneGrid()
{
}

void SceneGrid::Init()
{
	CreateUI();
	CreateMap();
	CreateMapObject();
	CreateHuman();
	CreateUndead();
	CreateTroll();
	CreateMonster();

	//CreateTest();
}

void SceneGrid::CreateMap()
{
	// Obj Map 테스트
	GameObject* pObjMap = factory.CreateObjMap("ObjMap", "./Resources/obj/Map/TestMap/", "tempMap2.obj");
	//GameObject* pObjMap = factory.CreateObjMap("ObjMap", "./Resources/obj/Map/TestMap/", "Terrain.obj");
}

void SceneGrid::CreateMapObject()
{
	// 아이템 정보를 통하여 맵에 게임오브젝트(어깨 방어구) 생성
	EquipmentShoulder* pShoulder = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X");
	pShoulder->Set(10, 10, 10, 10, eChrType_Human);
	GameObject* pGOShoulder = factory.CreateEquipmentToMap(pShoulder, Vector3(3, 10, -8), Vector3(-260, 15, -255));

	EquipmentHelmet* pHelmet = new EquipmentHelmet("Equipment_Helmet", "Helmet_01.X");
	pHelmet->Set(10, 10, 10, 10, eChrType_Human);
	GameObject* pGOHelmet = factory.CreateEquipmentToMap(pHelmet, Vector3(3, 10, -8), Vector3(-260, 15, -253));

	EquipmentShield* pShield = new EquipmentShield("Equipment_Shield", "Shield_01.X");
	GameObject* pGOShield = factory.CreateEquipmentToMap(pShield, Vector3(3, 10, -8), Vector3(-260, 15, -251));

	EquipmentWeapon* pWeaponR = new EquipmentWeapon("Equipment_weapon", "Sword_01.X");
	GameObject* pGOWeapon = factory.CreateEquipmentToMap(pWeaponR, Vector3(3, 10, -8), Vector3(-260, 15, -249));
}

void SceneGrid::CreateHuman()
{
	GameObject* pGOHuman = factory.CreateCharacter("human_01", "Resources/character/human_01/", "human_01.X", Vector3(-260, 15, -260), new ComHuman01("ComCharacter"));

	// 이 게임 오브젝트는 장비 장착 가능
	ComChrEquipment* pEquipment = (ComChrEquipment*)pGOHuman->GetComponent("ComChrEquipment");

	// 카메라
	Camera::GetInstance()->SetTarget(&pGOHuman->transform->GetPosition());
}

void SceneGrid::CreateUndead()
{
	GameObject* pGOUndead = factory.CreateCharacter("undead_01", "Resources/character/undead_01/", "undead_01.X", Vector3(-260, 15, -261), new ComUndead01("ComCharacter"));

	// 이 게임 오브젝트는 장비 장착 가능
	ComChrEquipment* pEquipment = (ComChrEquipment*)pGOUndead->GetComponent("ComChrEquipment");

	EquipmentShoulder* pShoulder = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X");
	pShoulder->Set(10, 10, 10, 10, eChrType_Undead);
	pShoulder->TextureName = "Resources/character/Equipment/shoulder_robe_b_03blue.png";
	pEquipment->Equip(pShoulder);

	EquipmentHelmet* pHelmet = new EquipmentHelmet("Equipment_Helmet", "Helmet_01.X");
	pHelmet->Set(10, 10, 10, 10, eChrType_Undead);
	pEquipment->Equip(pHelmet);

	EquipmentShield* pShield = new EquipmentShield("Equipment_Shield", "Shield_01.X");
	pShield->Set(10, eChrType_Undead);
	pEquipment->Equip(pShield);

	EquipmentWeapon* pWeaponR = new EquipmentWeapon("Equipment_weapon", "Sword_01.X");
	pWeaponR->Set(10, 20, eChrType_Undead);
	pEquipment->Equip(pWeaponR);
}

void SceneGrid::CreateTroll()
{
	GameObject* pGOTroll = factory.CreateCharacter("troll_01", "Resources/character/troll_01/", "troll_01.X", Vector3(-260, 15, -262), new ComTroll01("ComCharacter"));
	
	// 이 게임 오브젝트는 장비 장착 가능
	ComChrEquipment* pEquipment = (ComChrEquipment*)pGOTroll->GetComponent("ComChrEquipment");

	// 휴먼 캐릭터 장비 장착 테스트(추후 게임 도중 장착으로 수정할 예정)
	EquipmentShoulder*pShoulder = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X");
	pShoulder->Set(10, 10, 10, 10, eChrType_Troll);
	pShoulder->TextureName = "Resources/character/Equipment/shoulder_robe_b_03blue.png";
	pEquipment->Equip(pShoulder);

	EquipmentHelmet* pHelmet = new EquipmentHelmet("Equipment_Helmet", "Helmet_01.X");
	pHelmet->Set(10, 10, 10, 10, eChrType_Troll);
	pEquipment->Equip(pHelmet);

	EquipmentShield* pShield = new EquipmentShield("Equipment_Shield", "Shield_01.X");
	pShield->Set(10, eChrType_Troll);
	pEquipment->Equip(pShield);

	EquipmentWeapon* pWeaponR = new EquipmentWeapon("Equipment_weapon", "Sword_01.X");
	pWeaponR->Set(10, 20, eChrType_Troll);
	pEquipment->Equip(pWeaponR);
}

void SceneGrid::CreateMonster()
{
	// 몬스터 생성 (smallderon_orange)
	GameObject* pGOMonX = factory.CreateMonster("Monster", "Resources/monster/smallderon/", "smallderon_orange.X", Vector3(-240, 15, -240),
		new ComSmallderonAI("ComChrControl"), GameObject::Find("undead_01"));
	pGOMonX = factory.CreateMonster("Monster", "Resources/monster/smallderon/", "smallderon_orange.X", Vector3(-243, 15, -243),
		new ComSmallderonAI("ComChrControl"), GameObject::Find("human_01"));
	pGOMonX = factory.CreateMonster("Monster", "Resources/monster/smallderon/", "smallderon_orange.X", Vector3(-246, 15, -246),
		new ComSmallderonAI("ComChrControl"), GameObject::Find("troll_01"));
}

void SceneGrid::CreateTest()
{
	GameObject* pGOGrid = factory.CreateGrid("Grid");

	factory.CreatePointLight(&Color(1.0f, 0.0f, 0.0f, 1.0f), -10.0f, 10.0f, -10.0f);

	// GameObject 테스트용
	GameObject* pGOTestCube = factory.CreateCubePN("GOTestCube");
	pGOTestCube->transform->SetPosition(0.0f, 0.5f, 1.0f);
	pGOTestCube->transform->SetScale(0.5f, 0.5f, 0.5f);
	ComTestCubeControl* pControl = new ComTestCubeControl("ComTestCubeControl");
	pGOTestCube->AddComponent(pControl);

	// ASE 파일
	AseLoader loader;

	/*// 캐릭터
	GameObject* pGOChr = loader.Load("Resources/ase/", "woman_01_all_stand.ase");
	pGOChr->transform->SetPosition(0, 0, 3);*/

	// testcode : 맵 생성 후 캐릭터 생성
	//GameObject* pGOChrX = factory.CreateFromXFile("Zealot", "Resources/obj/zealot/", "zealot.X", Vector3(0, 0, 3));
	//GameObject* pGOChrX2 = factory.CreateFromXFile("Zealot", "Resources/obj/zealot/", "zealot.X", Vector3(0, 0, 4));

	// box1
	GameObject* pGOBox1 = loader.Load("Resources/obj/", "box.ase");
	pGOBox1->transform->SetPosition(-1, 0, 0);

	// box2
	GameObject* pGOBox2 = loader.Load("Resources/obj/", "box02.ase");
	pGOBox2->transform->SetPosition(1, 0, 0);
	ComRenderAse* pRenderAse = (ComRenderAse*)pGOBox2->GetComponent("ComRenderAse");
	pRenderAse->IsRenderBumpmap = true;
	pRenderAse->m_pTexBump = Assets::GetTexture(L"Resources/obj/WoodPlanksBare0102_6_L_NRM.jpg");

	// box3 to stone texture
	GameObject* pGOBox3 = loader.Load("Resources/obj/", "box02.ase");
	pGOBox3->transform->SetPosition(3, 0, 0);
	pRenderAse = (ComRenderAse*)pGOBox3->GetComponent("ComRenderAse");
	pRenderAse->IsRenderBumpmap = true;
	pRenderAse->m_mtlTex.pTexture = Assets::GetTexture(L"Resources/shaderResource/FieldStone/Fieldstone_DM.tga");
	pRenderAse->m_pTexBump = Assets::GetTexture(L"Resources/shaderResource/FieldStone/Fieldstone_NM.tga");
	pRenderAse->m_pTexSpecular = Assets::GetTexture(L"Resources/shaderResource/FieldStone/Fieldstone_SM.tga");

	GameObject* pGOBox4 = factory.CreateGameObject("Box4");
	ComRenderPOM* comPOM = new ComRenderPOM("ComRenderPOM");
	comPOM->m_vertices = pRenderAse->m_vertices;
	pGOBox4->AddComponent(comPOM);
	pGOBox4->transform->SetPosition(-1, 0, -2);

	GameObject* pGOBox5 = factory.CreateGameObject("Box5");
	comPOM = new ComRenderPOM("ComRenderPOM");
	comPOM->m_vertices = pRenderAse->m_vertices;
	comPOM->g_nCurrentTechniqueID = BUMPMAP;
	pGOBox5->AddComponent(comPOM);
	pGOBox5->transform->SetPosition(1, 0, -2);

	// 파티클
	GameObject* pGOParticle = factory.CreateGameObject("Particle");
	pGOParticle->AddComponent(new ComParticle("ComParticle"));

	// 포스트 이펙트 게임오브젝트를 렌더링에서 빼기 위해
	ComPostProcess* comPostProcess = new ComPostProcess("ComPostProcess");
	for (auto & o : GameObject::m_mapGameObjects) comPostProcess->m_listRenderObject.push_back(o.second);

	// 포스트 이펙트
	GameObject* pPostEffect = factory.CreateGameObject("PostEffect");
	pPostEffect->IsAlwaysRender = true;
	pPostEffect->AddComponent(comPostProcess);

}

void SceneGrid::CreateUI()
{
	GameObject* pUIBar = factory.CreateUIDialog("testUI", 20.0f, 20.0f);
	ComDialog* uiDialog = (ComDialog*)pUIBar->GetComponent("ComDialog");

	//((ComDialog*)uiDialog)->AddButton(1, "Resources/ui/btn-med-up.png", "Resources/ui/btn-med-over.png", "Resources/ui/btn-med-down.png");
	//((ComDialog*)uiDialog)->AddText(3, Assets::GetFont(Assets::FontType_NORMAL), "한글test123!@#");
	//pComDialog->GetText(3)->SetSize(Vector2(140.0f, 40.0f));
	//((ComDialog*)uiDialog)->AddProgressBar(0, "Resources/ui/6.tga");
	//((ComDialog*)uiDialog)->AddCheckBox(10, "Resources/ui/Box.png", "Resources/ui/Checked.png", Assets::GetFont(Assets::FontType_NORMAL), "testCheckBox");

	//pComDialog->GetButton(1)->SetPosition(Vector3(50.0f, 0.0f, 0.0f));
	//pComDialog->AddImage(2, "Resources/ui/btn-tower-up.png");

	/*GameObject* pTexture = factory.CreateGameObject("Texture");
	pTexture->transform->SetPosition(Vector3(-260, 15, -262));
	
	ComTexture* pComTex = new ComTexture("ComTexture");
	pComTex->Set(Assets::GetTexture(L"Resources/ui/6.tga"), true);
	pTexture->AddComponent(pComTex);*/

	/*uiDialog->AddRadioButton(100, "Resources/ui/Box.png", "Resources/ui/Checked.png", Assets::GetFont(Assets::FontType_NORMAL), "radio1");
	uiDialog->GetRadioButton(100)->AddRadioButton("radio2");
	uiDialog->GetRadioButton(100)->AddRadioButton("radio3");
	uiDialog->GetRadioButton(100)->AddRadioButton("radio4");*/
}