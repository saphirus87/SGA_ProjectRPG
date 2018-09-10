#include "stdafx.h"
#include "SceneRPG.h"
#include "../Application/ComTestCubeControl.h"
#include "../Application/ComHuman.h"
#include "../Application/ComChrEquipment.h"
#include "../Application/ComUndead.h"
#include "../Application/ComChrControl.h"
#include "../Application/ComTroll.h"
#include "../Application/ComSmallderonAI.h"
#include "../Application/ItemInfo.h"
#include "../Application/ComEquipment.h"
#include "../Application/ComFollowTarget.h"
#include "../Application/ComUIInventory.h"
#include "../Application/ComUICharacterInfo.h"

SceneRPG::SceneRPG(CString szName) : Scene(szName), 
	IsGameEnd(false),
	iLoadingPer(-1),
	m_pLoadingUI(NULL),
	m_pLoadingBar(NULL)
{
}

SceneRPG::~SceneRPG()
{
}

void SceneRPG::Init()
{
	GameObject* pLoadingUI = GameObject::Find("ScreenUI");
	m_pLoadingUI = (ComDialog*)pLoadingUI->GetComponent("ComDialog");

	m_pLoadingBar = m_pLoadingUI->GetProgressBar(eUI_LoadingBar);
	m_pLoadingBar->SetMaxValue(100);
	m_pLoadingBar->SetCurValue(0);
}

void SceneRPG::Update()
{
	Scene::Update();

	if (iLoadingPer >= 7)
	{
		// 로딩 완료
		m_pLoadingUI->SetIsVisible(false);
		return;
	}

	if (iLoadingPer == 0)
	{
		CreateUI();
		m_pLoadingBar->SetCurValue(10);
	}
		
	if (iLoadingPer == 1)
	{
		CreateMap();
		m_pLoadingBar->SetCurValue(30);
	}

	if (iLoadingPer == 2)
	{
		CreateMapObject();
		m_pLoadingBar->SetCurValue(40);
	}

	if (iLoadingPer == 3)
	{
		CreateHuman();
		m_pLoadingBar->SetCurValue(60);
	}

	if (iLoadingPer == 4)
	{
		CreateUndead();
		m_pLoadingBar->SetCurValue(80);
	}

	if (iLoadingPer == 5)
	{
		CreateTroll();
		m_pLoadingBar->SetCurValue(95);
	}

	if (iLoadingPer == 6)
	{
		CreateMonster();
		m_pLoadingBar->SetCurValue(100);
	}

	++iLoadingPer;

	//CreateTest();
}

void SceneRPG::CreateMap()
{
	// Obj Map 테스트
	GameObject* pObjMap = factory.CreateObjMap("ObjMap", "./Resources/obj/Map/TestMap/", "tempMap2.obj");
	//GameObject* pObjMap = factory.CreateObjMap("ObjMap", "./Resources/obj/Map/TestMap/", "Terrain.obj");
}

void SceneRPG::CreateMapObject()
{
	// 휴먼 장비들
	// 아이템 정보를 통하여 맵에 게임오브젝트(어깨 방어구) 생성
	EquipmentShoulder* pShoulder = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X", "Resources/ui/icon_shoulder_1.png");
	pShoulder->UID = 1;
	pShoulder->Set(10, 10, 1, 1, eChrType_Human);
	factory.CreateEquipmentToMap(pShoulder, Vector3(3, 10, -8), Vector3(-260, 15, -255));

	EquipmentHelmet* pHelmet = new EquipmentHelmet("Equipment_Helmet", "Helmet_01.X", "Resources/ui/icon_helmet_1.png");
	pHelmet->UID = 2;
	pHelmet->Set(10, 10, 1, 1, eChrType_Human);
	factory.CreateEquipmentToMap(pHelmet, Vector3(3, 10, -8), Vector3(-260, 15, -253));

	EquipmentShield* pShield = new EquipmentShield("Equipment_Shield", "Shield_01.X", "Resources/ui/icon_shield_1.png");
	pShield->UID = 3;
	pShield->Set(10, eChrType_Human);
	factory.CreateEquipmentToMap(pShield, Vector3(3, 10, -8), Vector3(-260, 15, -251));

	EquipmentWeapon* pWeaponR = new EquipmentWeapon("Equipment_weapon", "Sword_01.X", "Resources/ui/icon_sword_1.png");
	pWeaponR->UID = 4;
	pWeaponR->Set(10, 20, eChrType_Human);
	factory.CreateEquipmentToMap(pWeaponR, Vector3(3, 10, -8), Vector3(-260, 15, -249));

	// 언데드 장비들
	pShoulder = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X", "Resources/ui/icon_shoulder_1.png");
	pShoulder->UID = 5;
	pShoulder->Set(10, 10, 1, 1, eChrType_Undead);
	pShoulder->TextureName = "Resources/character/Equipment/shoulder_robe_b_03blue.png";
	factory.CreateEquipmentToMap(pShoulder, Vector3(3, 10, -8), Vector3(-255, 15, -255));

	pHelmet = new EquipmentHelmet("Equipment_Helmet", "Helmet_01.X", "Resources/ui/icon_helmet_1.png");
	pHelmet->UID = 6;
	pHelmet->Set(10, 10, 1, 1, eChrType_Undead);
	factory.CreateEquipmentToMap(pHelmet, Vector3(3, 10, -8), Vector3(-255, 15, -253));

	pShield = new EquipmentShield("Equipment_Shield", "Shield_01.X", "Resources/ui/icon_shield_1.png");
	pShield->UID = 7;
	pShield->Set(10, eChrType_Undead);
	factory.CreateEquipmentToMap(pShield, Vector3(3, 10, -8), Vector3(-255, 15, -251));

	pWeaponR = new EquipmentWeapon("Equipment_weapon", "Sword_01.X", "Resources/ui/icon_sword_1.png");
	pWeaponR->UID = 8;
	pWeaponR->Set(10, 20, eChrType_Undead);
	factory.CreateEquipmentToMap(pWeaponR, Vector3(3, 10, -8), Vector3(-255, 15, -249));
}

void SceneRPG::CreateHuman()
{
	GameObject* pGOHuman = factory.CreateCharacter("human_01", "Resources/character/human_01/", "human_01.X", 
		"Human", Vector3(-260, 10.7184200, -260), new ComHuman("ComCharacter"));

	// 카메라
	Camera::GetInstance()->SetTarget(&pGOHuman->transform->GetPosition());

	ComChrEquipment* pEquipment = (ComChrEquipment*)pGOHuman->GetComponent("ComChrEquipment");

	GameObject* pCharacterInfo = GameObject::Find("CharacterInfoUI");
	ComUICharacterInfo* pComChrInfo = (ComUICharacterInfo*)pCharacterInfo->GetComponent("ComUICharacterInfo");
	pComChrInfo->SetChrEquip(pEquipment);
	pComChrInfo->UpdateIcons();
}

void SceneRPG::CreateUndead()
{
	GameObject* pGOUndead = factory.CreateCharacter("undead_01", "Resources/character/undead_01/", "undead_01.X", 
		"Undead", Vector3(-260, 10.3810062, -261), new ComUndead("ComCharacter"));

	/*GameObject* pTexture = factory.CreateGameObject("Texture");
	pTexture->transform->SetPosition(pGOUndead->transform->GetPosition().x, pGOUndead->transform->GetPosition().y + 2, pGOUndead->transform->GetPosition().z);
	
	ComTexture* pComTex = new ComTexture("ComTexture");
	pComTex->Set(Assets::GetTexture(L"Resources/ui/name/undead_name.png"), true);
	pTexture->AddComponent(pComTex);*/
}

void SceneRPG::CreateTroll()
{
	GameObject* pGOTroll = factory.CreateCharacter("troll_01", "Resources/character/troll_01/", "troll_01.X", 
		"Troll", Vector3(-260, 10.0443125, -262), new ComTroll("ComCharacter"));
	
	// 이 게임 오브젝트는 장비 장착 가능
	ComChrEquipment* pEquipment = (ComChrEquipment*)pGOTroll->GetComponent("ComChrEquipment");

	// 장비 장착 테스트(추후 게임 도중 장착으로 수정할 예정)
	EquipmentShoulder*pShoulder = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X", "Resources/ui/icon_shoulder_1.png");
	pShoulder->UID = 9;
	pShoulder->Set(10, 10, 1, 1, eChrType_Troll);
	pShoulder->TextureName = "Resources/character/Equipment/shoulder_robe_b_03blue.png";
	pEquipment->Equip(pShoulder);

	EquipmentHelmet* pHelmet = new EquipmentHelmet("Equipment_Helmet", "Helmet_01.X", "Resources/ui/icon_helmet_1.png");
	pHelmet->UID = 10;
	pHelmet->Set(10, 10, 1, 1, eChrType_Troll);
	pEquipment->Equip(pHelmet);

	EquipmentShield* pShield = new EquipmentShield("Equipment_Shield", "Shield_01.X", "Resources/ui/icon_shield_1.png");
	pShield->UID = 11;
	pShield->Set(10, eChrType_Troll);
	pEquipment->Equip(pShield);

	EquipmentWeapon* pWeaponR = new EquipmentWeapon("Equipment_weapon", "Sword_01.X", "Resources/ui/icon_sword_1.png");
	pWeaponR->UID = 12;
	pWeaponR->Set(10, 20, eChrType_Troll);
	pEquipment->Equip(pWeaponR);
}

void SceneRPG::CreateMonster()
{
	StatusInfo monStatus;
	monStatus.HP = 50;
	monStatus.HPMAX = 50;
	monStatus.ATK_PHY = 5;

	// 몬스터 생성 (smallderon_orange)
	GameObject* pGOMonX = factory.CreateMonster("폭염몬", "Resources/monster/smallderon/", "smallderon_orange.X", Vector3(-243, 7.7184200, -240),
		new ComSmallderonAI("ComCharacter"), monStatus);
	pGOMonX = factory.CreateMonster("폭염몬", "Resources/monster/smallderon/", "smallderon_orange.X", Vector3(-243, 10.7184200, -243),
		new ComSmallderonAI("ComCharacter"), monStatus);
	pGOMonX = factory.CreateMonster("폭염몬", "Resources/monster/smallderon/", "smallderon_orange.X", Vector3(-246, 9.7184200, -246),
		new ComSmallderonAI("ComCharacter"), monStatus);

	monStatus.HP = 60;
	monStatus.HPMAX = 60;
	monStatus.ATK_PHY = 6;

	pGOMonX = factory.CreateMonster("폭염몬", "Resources/monster/smallderon/", "smallderon_orange.X", Vector3(-220, 10.7184200, -220),
		new ComSmallderonAI("ComCharacter"), monStatus);
	pGOMonX = factory.CreateMonster("폭염몬", "Resources/monster/smallderon/", "smallderon_orange.X", Vector3(-223, 10.7184200, -223),
		new ComSmallderonAI("ComCharacter"), monStatus);
	pGOMonX = factory.CreateMonster("폭염몬", "Resources/monster/smallderon/", "smallderon_orange.X", Vector3(-226, 10.7184200, -226),
		new ComSmallderonAI("ComCharacter"), monStatus);
}

void SceneRPG::CreateTest()
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

void SceneRPG::CreateUI()
{
	GameObject* pUIBar = factory.CreateUIDialog("testUI", Vector3(20.0f, 20.0f, 1.0f));
	ComDialog* uiDialog = (ComDialog*)pUIBar->GetComponent("ComDialog");
	uiDialog->SetIsVisible(true);

	//((ComDialog*)uiDialog)->AddButton(1, "Resources/ui/btn-med-up.png", "Resources/ui/btn-med-over.png", "Resources/ui/btn-med-down.png");
	//((ComDialog*)uiDialog)->AddText(3, Assets::GetFont(Assets::FontType_NORMAL), "한글test123!@#");
	//pComDialog->GetText(3)->SetSize(Vector2(140.0f, 40.0f));
	//((ComDialog*)uiDialog)->AddProgressBar(0, "Resources/ui/6.tga");
	//((ComDialog*)uiDialog)->AddCheckBox(10, "Resources/ui/Box.png", "Resources/ui/Checked.png", Assets::GetFont(Assets::FontType_NORMAL), "testCheckBox");

	//pComDialog->GetButton(1)->SetPosition(Vector3(50.0f, 0.0f, 0.0f));
	//pComDialog->AddImage(2, "Resources/ui/btn-tower-up.png");

	// -260, 15, -261
	/*GameObject* pTexture = factory.CreateGameObject("Texture");
	pTexture->transform->SetPosition(Vector3(-260, 10, -261));
	
	ComTexture* pComTex = new ComTexture("ComTexture");
	pComTex->Set(Assets::GetTexture(L"Resources/ui/name/undead_name.png"), true);
	pTexture->AddComponent(pComTex);*/

	/*uiDialog->AddRadioButton(100, "Resources/ui/Box.png", "Resources/ui/Checked.png", Assets::GetFont(Assets::FontType_NORMAL), "radio1");
	uiDialog->GetRadioButton(100)->AddRadioButton("radio2");
	uiDialog->GetRadioButton(100)->AddRadioButton("radio3");
	uiDialog->GetRadioButton(100)->AddRadioButton("radio4");*/

	GameObject* pUIInven = factory.CreateUIDialog("InvenUI_Undead", Vector3(120.0f, 120.0f, 2.0f));
	ComUIInventory* pComInven = new ComUIInventory("ComUIInventory");
	pComInven->SetInvenSize(16);
	pUIInven->AddComponent(pComInven);
	uiDialog = (ComDialog*)pUIInven->GetComponent("ComDialog");
	RECT rc;
	SetRect(&rc, 80, 10, 280, 50);

	uiDialog->SetToggleKey('I');
	uiDialog->SetMoveable(true);
	uiDialog->SetDragArea(rc);
	uiDialog->AddImage(0, "Resources/ui/ui-backpackbackground.png");
	uiDialog->AddButton(2, "Resources/ui/ui-panel-minimizebutton-up.png", "Resources/ui/ui-panel-minimizebutton-up.png",
		"Resources/ui/ui-panel-minimizebutton-down.png", pComInven, "InvenClose");
	//uiDialog->GetButton(2)->SetScale(Vector3(2.5f, 2.5f, 1.0f));
	uiDialog->GetButton(2)->SetPosition(Vector3(230, 8, 0));

	// 캐릭터 장비 정보 창 추가(캐릭터 mesh 보이게 추가 필요)
	GameObject* pUICharacterInfo = factory.CreateUIDialog("CharacterInfoUI", Vector3(200.0f, 200.0f, 0));
	ComUICharacterInfo* pComCharacterInfo = new ComUICharacterInfo("ComUICharacterInfo");
	uiDialog = (ComDialog*)pUICharacterInfo->GetComponent("ComDialog");
	pUICharacterInfo->AddComponent(pComCharacterInfo);
	SetRect(&rc, 80, 10, 380, 100);

	uiDialog->SetToggleKey('C');
	uiDialog->SetMoveable(true);
	uiDialog->SetDragArea(rc);
	uiDialog->AddImage(0, "Resources/ui/CharacterFrame.png");
	uiDialog->AddButton(1, "Resources/ui/ui-panel-minimizebutton-up.png", "Resources/ui/ui-panel-minimizebutton-up.png",
		"Resources/ui/ui-panel-minimizebutton-down.png", pComCharacterInfo, "CharacterInfoClose");
	uiDialog->GetButton(1)->SetPosition(Vector3(330, 16, 0));

	GameObject* pCharacter3 = GameObject::Find("troll_01");
}