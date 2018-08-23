#pragma once
#include "stdafx.h"
#include "StatusInfo.h"

class AttackHandler : public ID3DXAnimationCallbackHandler
{
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};

class ComChrEquipment;

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

	// Key Frame Event를 넣기 위해 애니메이션을 압축하고 새로 등록(Register)합니다. 
	void AnimationCompress();

	// 타겟을 공격합니다.
	void AttackTarget(ComCharacter* pTarget);
	
	// 방어를 합니다.
	void Defence(int dmg);
	
	// 죽음을 검사합니다.
	bool CheckDeath();

	// UI 관련
	virtual void UpdateHPBar() {}

protected:
	void Init();

protected:
	// 캐릭터 타입
	eChrType m_eType;

	ComRenderSkinnedMesh * m_pAnimation;
	
	// 애니메이션
	vector<LPD3DXKEYFRAMEDANIMATIONSET> m_vecKeyFrameAnimSet;

	// 공격할 타겟
	ComCharacter* m_pAttackTarget;

	// 능력치
	StatusInfo m_status;

	// 장비 장착
	ComChrEquipment* m_pChrEquipment;
};