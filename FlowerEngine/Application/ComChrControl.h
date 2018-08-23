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

	void Init();
	void SetState(int iIndex);
		
	//상태 기계
	virtual void Stand();
	virtual void Walk(float fDeltaZ);
	virtual void Attack1();

	// 이 객체가 픽킹되었는지 여부를 검사합니다. (캐릭터, 몬스터, 맵)
	void CheckPickingChr();
	void CheckPickingMon();
	void CheckPickingMap();

	// 이 객체가 픽킹되었는지 여부
	bool IsPicking;
	ComRenderSkinnedMesh * pAnimation;

	// 공격하고자 하는 타겟
	ComCharacter* pAttackTarget;
	void CancleAttackTarget();
	// 대상을 바라봅니다.
	void LookatTarget();
	// 공격대상이 죽었을 때 처리를 합니다.
	void CheckAttackTargetDeath();

protected:
	void GetHeight();
	void MoveToPoint();
	
protected:
	ComObjMap * m_pMap;
	IChrState * m_pCurrentState;
	
	vector<IChrState*> m_vecState;

	ComFollowTarget* m_pFollow;
	ComCharacter* m_pCharacter;

	// 이동하고하 하는 위치
	Vector3 vMoveToPoint;
	// 특정 좌표로 이동 여부
	bool IsMoveToPoint;
};