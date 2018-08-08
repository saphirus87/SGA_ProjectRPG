#pragma once
#include "stdafx.h"

class ComObjMap;

class ComSmallderon : public Component
{
public:
	ComSmallderon(CString szName);
	~ComSmallderon();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ComRenderSkinnedMesh * m_pAnimation;
	ComObjMap* m_pMap;

};

