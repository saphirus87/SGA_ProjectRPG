#include "stdafx.h"
#include "SceneGrid.h"
#include "../Application/ComTestCubeControl.h"
#include "../Application/ComHuman01.h"
#include "../Application/ComChrEquipment.h"
#include "../Application/ComUndead01.h"
#include "../Application/ComChrControl.h"
#include "../Application/ComTroll01.h"
#include "../Application/ComSmallderon.h"
#include "../Application/ItemInfo.h"
#include "../Application/ComEquipment.h"

SceneGrid::SceneGrid(CString szName) : Scene(szName)
{
}

SceneGrid::~SceneGrid()
{
}

void SceneGrid::Init()
{
	CreateMap();
	CreateMapObject();

	// testcode : �� ���� �� ĳ���� ����
	//GameObject* pGOChrX = factory.CreateFromXFile("Zealot", "Resources/obj/zealot/", "zealot.X", Vector3(0, 0, 3));
	//GameObject* pGOChrX2 = factory.CreateFromXFile("Zealot", "Resources/obj/zealot/", "zealot.X", Vector3(0, 0, 4));

	CreateHuman01();
	CreateUndead01();
	CreateTroll01();
	//CreateMonster();
	CreateTest();
}

void SceneGrid::CreateMap()
{
	// Obj Map �׽�Ʈ
	GameObject* pObjMap = factory.CreateObjMap("ObjMap", "./Resources/obj/Map/TestMap/", "tempMap2.obj");
	//GameObject* pObjMap = factory.CreateObjMap("ObjMap", "./Resources/obj/Map/TestMap/", "Terrain.obj");
}

void SceneGrid::CreateMapObject()
{
	// ������ ������ ���Ͽ� �ʿ� ���ӿ�����Ʈ(��� ��) ����
	EquipmentShoulder* pShoulder = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X");
	pShoulder->Set(10, 10, 10, 10, eChrType_Human);

	GameObject* pGOShoulder = factory.CreateEquipment(pShoulder, Vector3(3, 10, -8));
	((ComEquipment*)pGOShoulder->GetComponent("ComEquipment"))->pItemInfo = pShoulder;
	pGOShoulder->transform->SetPosition(-260, 15, -255);
	ComCollider* pCollider = new ComCollider("ComCollider");
	pGOShoulder->AddComponent(pCollider);
	pCollider->Set(Vector3(0.1, 0.1, 0.1), false);
}

void SceneGrid::CreateHuman01()
{
	GameObject* pGOChrX3 = factory.CreateFromXFile("human_01", "Resources/character/human_01/", "human_01.X", Vector3(-260, 15, -260));
	// �� ���� ������Ʈ�� �޸�
	pGOChrX3->AddComponent(new ComHuman01("ComHuman01"));
	// �� ���� ������Ʈ�� ��Ʈ�� ����
	pGOChrX3->AddComponent(new ComChrControl("ComChrControl"));
	// �� ���� ������Ʈ�� ��� ���� ����
	ComChrEquipment* pEquipment = new ComChrEquipment("ComChrEquipment");
	pGOChrX3->AddComponent(pEquipment);
	// �� ���� ������Ʈ�� �浹üũ ����
	ComCollider* pCollider = new ComCollider("ComCollider");
	pGOChrX3->AddComponent(pCollider);
	pCollider->Set(Vector3(0.3, 0.6, 0.3), false);

	// ��� ����
	EquipmentHelmet* pHelmet = new EquipmentHelmet("Equipment_Helmet", "Helmet_01.X");
	pHelmet->Set(10, 10, 10, 10, eChrType_Human);
	pEquipment->Equip(pHelmet);

	EquipmentShield* pShield = new EquipmentShield("Equipment_Shield", "Shield_01.X");
	pEquipment->Equip(pShield);

	EquipmentWeapon* pWeaponR = new EquipmentWeapon("Equipment_weapon", "Sword_01.X");
	pEquipment->Equip(pWeaponR);

	EquipmentWeapon* pWeaponL = new EquipmentWeapon("Equipment_weapon", "Sword_01.X");
	pWeaponL->Type = eItem_WeaponL;
	pEquipment->Equip(pWeaponL);

	// ī�޶�
	Camera::GetInstance()->SetTarget(&pGOChrX3->transform->GetPosition());
}

void SceneGrid::CreateUndead01()
{
	GameObject* pGOChrX4 = factory.CreateFromXFile("undead_01", "Resources/character/undead_01/", "undead_01.X", Vector3(-260, 15, -261));
	pGOChrX4->AddComponent(new ComUndead01("ComUndead01"));
	pGOChrX4->AddComponent(new ComChrControl("ComChrControl"));
	ComChrEquipment* pEquipment = new ComChrEquipment("ComChrEquipment");
	pGOChrX4->AddComponent(pEquipment);

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

void SceneGrid::CreateTroll01()
{
	GameObject* pGOChrX5 = factory.CreateFromXFile("troll_01", "Resources/character/troll_01/", "troll_01.X", Vector3(-260, 15, -262));
	pGOChrX5->AddComponent(new ComTroll01("ComTroll01"));
	pGOChrX5->AddComponent(new ComChrControl("ComChrControl"));
	ComChrEquipment* pEquipment = new ComChrEquipment("ComChrEquipment");
	pGOChrX5->AddComponent(pEquipment);

	// �޸� ĳ���� ��� ���� �׽�Ʈ(���� ���� ���� �������� ������ ����)
	EquipmentShoulder*pShoulder = new EquipmentShoulder("Equipment_shoulder_ItemName01", "shoulder_01.X");
	pShoulder->Set(10, 10, 10, 10, eChrType_Troll);
	pShoulder->Name = "Equipment_shoulder_ItemName01";
	pShoulder->FolderPath = "Resources/character/Equipment/";
	pShoulder->XFileName = "shoulder_01.X";
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
	// ���� ����
	GameObject* pGOMonX = factory.CreateFromXFile("smallderon_orange", "Resources/monster/smallderon/", "smallderon_orange.X", Vector3(0, 15, 9));
	pGOMonX->AddComponent(new ComSmallderon("ComSmallderon"));
}

void SceneGrid::CreateTest()
{
	GameObject* pGOGrid = factory.CreateGrid("Grid");

	factory.CreatePointLight(&Color(1.0f, 0.0f, 0.0f, 1.0f), -10.0f, 10.0f, -10.0f);

	// GameObject �׽�Ʈ��
	GameObject* pGOTestCube = factory.CreateCubePN("GOTestCube");
	pGOTestCube->transform->SetPosition(0.0f, 0.5f, 1.0f);
	pGOTestCube->transform->SetScale(0.5f, 0.5f, 0.5f);
	ComTestCubeControl* pControl = new ComTestCubeControl("ComTestCubeControl");
	pGOTestCube->AddComponent(pControl);

	// ASE ����
	AseLoader loader;

	/*// ĳ����
	GameObject* pGOChr = loader.Load("Resources/ase/", "woman_01_all_stand.ase");
	pGOChr->transform->SetPosition(0, 0, 3);*/

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

	// ��ƼŬ
	GameObject* pGOParticle = factory.CreateGameObject("Particle");
	pGOParticle->AddComponent(new ComParticle("ComParticle"));

	// ����Ʈ ����Ʈ ���ӿ�����Ʈ�� ���������� ���� ����
	ComPostProcess* comPostProcess = new ComPostProcess("ComPostProcess");
	for (auto & o : GameObject::m_mapGameObjects) comPostProcess->m_listRenderObject.push_back(o.second);

	// ����Ʈ ����Ʈ
	GameObject* pPostEffect = factory.CreateGameObject("PostEffect");
	pPostEffect->IsAlwaysRender = true;
	pPostEffect->transform->SetPosition(0, 0, -2);
	pPostEffect->AddComponent(comPostProcess);

}
