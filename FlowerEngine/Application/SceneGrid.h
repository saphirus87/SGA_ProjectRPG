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
	// 2. ĳ���͸� �����մϴ�.
	void CreateCharacter();
	// 3. ���͸� �����մϴ�.
	void CreateMonster();
	// 4. �׽�Ʈ ��ü���� �����մϴ�.
	void CreateTest();

private:
	FactoryGameObject factory;
	D3DLIGHT9 m_lightPoint; // ����Ʈ ����Ʈ
};

