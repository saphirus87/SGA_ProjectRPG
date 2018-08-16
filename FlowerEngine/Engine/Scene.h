#pragma once
#include "stdafx.h"

class Scene
{
public:
	Scene(CString szName);
	virtual ~Scene();

	virtual void Init() = 0;

	// �ʿ�� ������ �ؼ� ���
	void Update();
	void Render();
	inline CString& Name() { return m_szName; }

	void CleanUpGameObject();

	void RenderObjects();

private:
	GameObject * m_pGOPostEffect = NULL;
	int m_iRenderObjCnt;

	CString m_szName;
	list<GameObject*> m_listRender;
	list<GameObject*> m_listRenderUI;
};

bool CompareZ(const GameObject* pGO1, const GameObject* pGO2);