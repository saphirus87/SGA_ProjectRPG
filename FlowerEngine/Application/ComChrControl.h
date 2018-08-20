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

	// 이 객체가 픽킹되었는지 여부를 검사합니다. (캐릭터, 몬스터)
	void CheckPickingChr();
	void CheckPickingMon();

	// 이 객체가 픽킹되었는지 여부
	bool IsPicking;
	ComRenderSkinnedMesh * pAnimation;

protected:
	void GetHeight();
	
protected:
	ComObjMap * m_pMap;
	IChrState * m_pCurrentState;

	Vector3 m_vecForward;			// 앞 방향 벡터

	vector<IChrState*> m_vecState;

	ComFollowTarget* m_pTarget;
	ComCharacter* m_pCharacter;
};