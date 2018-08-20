#pragma once
#include "stdafx.h"

// 캐릭터의 공통요소입니다.
class ComCharacter : public Component
{
public:
	ComCharacter(CString szName);
	virtual ~ComCharacter();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

protected:
	void Init();

protected:
	ComRenderSkinnedMesh * m_pAnimation;
};

