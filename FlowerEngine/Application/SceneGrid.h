#pragma once
#include "stdafx.h"

// 연습장
class SceneGrid : public Scene
{
public:
	SceneGrid(CString szName);
	~SceneGrid();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;

private:
	// 1. 맵을 생성합니다.
	void CreateMap();
	// 2. 맵 오브젝트들을 생성합니다.
	void CreateMapObject();
	// 3. 캐릭터를 생성합니다.
	void CreateHuman();
	void CreateUndead();
	void CreateTroll();
	// 4. 몬스터를 생성합니다.
	void CreateMonster();
	// 5. 테스트 객체들을 생성합니다.
	void CreateTest();

	// 6. Test : UI 생성
	void CreateUI();

private:
	FactoryGameObject factory;
	D3DLIGHT9 m_lightPoint; // 포인트 라이트
};

