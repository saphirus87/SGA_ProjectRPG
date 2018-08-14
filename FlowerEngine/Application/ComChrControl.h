#pragma once
#include "stdafx.h"

class ComObjMap;
class ComTerrain;
class ChrState;

class ComChrControl : public Component
{
public:
	ComChrControl(CString szName);
	~ComChrControl();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	void Move();

private:
	ComObjMap * m_pMap;

	Vector3* m_pos;					// 캐릭터 위치
	Vector3 m_vecForward;			// 앞 방향 벡터
	bool m_isMoving;				// 움직임 변경여부
	bool m_isRotating;				// 방향 변경여부

public:
	bool GetMoving(bool isMoving);
};