#pragma once
#include "stdafx.h"
#include "StatusInfo.h"

class AttackHandler : public ID3DXAnimationCallbackHandler
{
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};

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

	// 타겟을 공격합니다.
	void AttackTarget(ComCharacter* pTarget);
	
	// 방어를 합니다.
	void Defence(int dmg);

	// 죽음을 검사합니다.
	bool CheckDeath();

protected:
	void Init();

protected:
	ComRenderSkinnedMesh * m_pAnimation;
	StatusInfo m_status;
};