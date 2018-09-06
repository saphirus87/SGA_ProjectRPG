#pragma once
#include "stdafx.h"

class ComObjMap;
class ComTerrain;
class ComFollowTarget;
class ComCharacter;
class IChrState;

class ComChrControl : public Component
{
public:
	ComChrControl(CString szName);
	virtual ~ComChrControl();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 초기화 관련
	void Init();
		
	//상태 기계
	void SetState(int iIndex);
	virtual void Stand();
	virtual void Walk(float fDeltaZ);
	virtual void Attack1();
	virtual void Death();

	// 이 객체가 픽킹되었는지 여부를 검사합니다. (캐릭터, 몬스터, 맵)
	void CheckPickingChr();
	bool CheckPickingMon();
	void CheckPickingMap();

	// 공격대상을 취소합니다.
	void CancleAttackTarget();
	// 대상을 바라봅니다.
	void LookatTarget();
	// 공격대상이 죽었을 때 처리를 합니다.
	void CheckAttackTargetDeath();

protected:
	// 맵에서 높이를 얻어옵니다.
	void GetHeight();
	// 맵을 클릭하면 해당 위치로 이동합니다.
	void MoveToPoint();
	// 공격할 대상을 찾습니다.
	void FindAttackTarget();

public:
	// 이 객체가 픽킹되었는지 여부
	bool IsPicking;
	ComRenderSkinnedMesh * pAnimation;
	// 공격하고자 하는 타겟
	ComCharacter* pAttackTarget;
	ComCharacter* m_pCharacter;
	// 상태들
	vector<IChrState*> m_vecState;
	// 현재 상태
	IChrState * m_pCurrentState;

protected:
	ComObjMap * m_pMap;
	// 타겟으로 따라감
	ComFollowTarget* m_pFollow;
	// 이동하고하 하는 위치
	Vector3 vMoveToPoint;
	// 특정 좌표로 이동 여부
	bool IsMoveToPoint;
	// 초기 캐릭터 셋팅시 땅 위에 있는지 여부
	bool IsGroud;
};