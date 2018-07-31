#pragma once
#include "stdafx.h"

enum eAniHuman
{
	eAni_Stand = 1,
	eAni_Walk = 0
};

class ComObjMap;

class ComHuman01 : public Component
{
public:
	ComHuman01(CString szName);
	~ComHuman01();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ComRenderSkinnedMesh * m_pAnimation;
	ComObjMap* m_pMap;
};

