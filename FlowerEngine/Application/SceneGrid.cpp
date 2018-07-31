#include "stdafx.h"
#include "SceneGrid.h"
#include "../Application/ComTestCubeControl.h"
#include "../Application/ComHuman01.h"
#include "../Application/ComUndead01.h"

SceneGrid::SceneGrid(CString szName) : Scene(szName)
{
}

SceneGrid::~SceneGrid()
{
}

void SceneGrid::Init()
{
	FactoryGameObject factory;
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

	// Obj Map 테스트
	GameObject* pObjMap = factory.CreateObjMap("ObjMap", "./Resources/obj/Map/TestMap/", "tempMap2.obj", "tempMap2.obj");
//	pObjMap->transform->SetPosition(-3, -5, -3);
	//pObjMap->transform->SetScale(0.5f, 0.5f, 0.5f);

	// 오브젝트들 생성
	GameObject* pGOEquipment = new GameObject("Equipment_shoulder");
	ComXMesh* pMesh = new ComXMesh("ComXMesh");
	pMesh->Load("Resources/character/Equipment/", "shoulder_01.X");
	pGOEquipment->AddComponent(pMesh);
	pGOEquipment->transform->SetPosition(0, 1, 0);

	// 맵 생성 후 캐릭터 생성
	GameObject* pGOChrX = factory.CreateFromXFile("Zealot", "Resources/obj/zealot/", "zealot.X", Vector3(0, 0, 3));
	GameObject* pGOChrX2 = factory.CreateFromXFile("Zealot", "Resources/obj/zealot/", "zealot.X", Vector3(0, 0, 4));
	GameObject* pGOChrX3 = factory.CreateFromXFile("human_01", "Resources/character/", "human_01.X", Vector3(0, 10, 5));
	pGOChrX3->AddComponent(new ComHuman01("ComHuman01"));
	GameObject* pGOChrX4 = factory.CreateFromXFile("undead_01", "Resources/character/undead_01/", "undead_01.X", Vector3(0, 15, 6));
	pGOChrX4->AddComponent(new ComUndead01("ComUndead01"));
		
	// 카메라
	Camera::GetInstance()->SetTarget(&pGOEquipment->transform->GetPosition());

	// 포스트 이펙트 게임오브젝트를 렌더링에서 빼기 위해
	ComPostProcess* comPostProcess = new ComPostProcess("ComPostProcess");
	for (auto & o : GameObject::m_mapGameObjects) comPostProcess->m_listRenderObject.push_back(o.second);

	// 포스트 이펙트
	GameObject* pPostEffect = factory.CreateGameObject("PostEffect");
	pPostEffect->transform->SetPosition(0, 0, -2);
	pPostEffect->AddComponent(comPostProcess);

}
