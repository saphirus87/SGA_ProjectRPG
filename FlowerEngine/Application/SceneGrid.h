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
	// 2. 캐릭터를 생성합니다.
	void CreateCharacter();
	// 3. 몬스터를 생성합니다.
	void CreateMonster();
	// 4. 테스트 객체들을 생성합니다.
	void CreateTest();

private:
	FactoryGameObject factory;
	D3DLIGHT9 m_lightPoint; // 포인트 라이트
};

