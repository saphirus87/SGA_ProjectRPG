#pragma once
#include "stdafx.h"

class Scene
{
public:
	Scene(CString szName);
	virtual ~Scene();

	virtual void Init() = 0;

	// 필요시 재정의 해서 사용
	void Update();
	void Render();
	inline CString& Name() { return m_szName; }

	void CleanUpGameObject();

private:
	CString m_szName;
	list<GameObject*> m_listRender;
	list<GameObject*> m_listRenderUI;
};

bool CompareZ(const GameObject* pGO1, const GameObject* pGO2);