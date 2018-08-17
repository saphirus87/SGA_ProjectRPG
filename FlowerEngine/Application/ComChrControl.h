#pragma once
#include "stdafx.h"

class ComObjMap;
class ComTerrain;
class ChrState;
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

	ComRenderSkinnedMesh * m_pAnimation;

	//상태 기계
	virtual void Stand();
	virtual void Walk(float fDeltaZ);
	virtual void Attack1();

protected:
	void GetHeight();
	
protected:
	ComObjMap * m_pMap;
	IChrState * m_pCurrentState;

	Vector3 m_vecForward;			// 앞 방향 벡터

	vector<IChrState*> m_vecState;
};