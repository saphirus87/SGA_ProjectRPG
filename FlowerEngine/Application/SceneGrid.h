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
	D3DLIGHT9 m_lightPoint; // 포인트 라이트
};

