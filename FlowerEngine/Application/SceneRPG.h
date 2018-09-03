#pragma once
#include "stdafx.h"

// WOW 모델을 사용한 롤 플레잉 게임씬
class SceneRPG : public Scene
{
public:
	SceneRPG(CString szName);
	~SceneRPG();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	void Update() override;

	// 게임이 끝났는지 여부
	bool IsGameEnd;

private:
	// 1. UI 생성
	void CreateUI();
	// 2. 맵을 생성합니다.
	void CreateMap();
	// 3. 맵 오브젝트들을 생성합니다.
	void CreateMapObject();
	// 4. 캐릭터를 생성합니다.
	void CreateHuman();
	void CreateUndead();
	void CreateTroll();
	// 5. 몬스터를 생성합니다.
	void CreateMonster();
	
	// 6. 테스트 객체들을 생성합니다.
	void CreateTest();

private:
	FactoryGameObject factory;
	D3DLIGHT9 m_lightPoint; // 포인트 라이트

	float fLoadingPer;
};

