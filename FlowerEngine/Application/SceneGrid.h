#pragma once
#include "stdafx.h"

// ������
class SceneGrid : public Scene
{
public:
	SceneGrid(CString szName);
	~SceneGrid();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;

private:
	// 1. ���� �����մϴ�.
	void CreateMap();
	// 2. �� ������Ʈ���� �����մϴ�.
	void CreateMapObject();
	// 3. ĳ���͸� �����մϴ�.
	void CreateHuman();
	void CreateUndead();
	void CreateTroll();
	// 4. ���͸� �����մϴ�.
	void CreateMonster();
	// 5. �׽�Ʈ ��ü���� �����մϴ�.
	void CreateTest();

	// 6. Test : UI ����
	void CreateUI();

private:
	FactoryGameObject factory;
	D3DLIGHT9 m_lightPoint; // ����Ʈ ����Ʈ
};

