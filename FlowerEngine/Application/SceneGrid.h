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
	D3DLIGHT9 m_lightPoint; // ����Ʈ ����Ʈ
};

