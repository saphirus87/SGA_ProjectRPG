#pragma once
#include "stdafx.h"

// WOW ���� ����� �� �÷��� ���Ӿ�
class SceneRPG : public Scene
{
public:
	SceneRPG(CString szName);
	~SceneRPG();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	void Update() override;

	// ������ �������� ����
	bool IsGameEnd;

private:
	// 1. UI ����
	void CreateUI();
	// 2. ���� �����մϴ�.
	void CreateMap();
	// 3. �� ������Ʈ���� �����մϴ�.
	void CreateMapObject();
	// 4. ĳ���͸� �����մϴ�.
	void CreateHuman();
	void CreateUndead();
	void CreateTroll();
	// 5. ���͸� �����մϴ�.
	void CreateMonster();
	
	// 6. �׽�Ʈ ��ü���� �����մϴ�.
	void CreateTest();

private:
	FactoryGameObject factory;
	D3DLIGHT9 m_lightPoint; // ����Ʈ ����Ʈ

	float fLoadingPer;
};

