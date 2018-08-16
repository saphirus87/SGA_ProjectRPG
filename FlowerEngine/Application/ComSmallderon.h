#pragma once
#include "stdafx.h"

class ComObjMap;
class ComFollowTarget;
class IChrState;

class ComSmallderon : public Component
{
public:
	ComSmallderon(CString szName);
	~ComSmallderon();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void Stand();
	void Walk();
	void Attack1();
private:
	ComObjMap* m_pMap;
	ComFollowTarget* m_pTarget;
	
	ComRenderSkinnedMesh * m_pAnimation;
	IChrState * m_pCurrentState;
};

