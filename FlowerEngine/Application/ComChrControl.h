#pragma once
#include "stdafx.h"

class ComObjMap;
class ComTerrain;
class ComFollowTarget;
class ComCharacter;

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
	
	// 이 객체가 픽킹되었는지 여부를 검사합니다. (캐릭터, 몬스터, 맵)
	void CheckPickingChr();
	void CheckPickingMap();

protected:
	// 맵을 클릭하면 해당 위치로 이동합니다.
	void MoveToPoint();
	
public:
	// 이 객체가 픽킹되었는지 여부
	bool IsPicking;
	ComRenderSkinnedMesh * pAnimation;
	ComCharacter* m_pCharacter;
	
protected:
	ComObjMap * m_pMap;
	// 이동하고하 하는 위치
	Vector3 vMoveToPoint;
	// 특정 좌표로 이동 여부
	bool IsMoveToPoint;

private:
	// 타겟으로 따라감
	ComFollowTarget* m_pFollow;
};