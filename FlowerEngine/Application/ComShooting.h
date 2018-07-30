#pragma once
#include "stdafx.h"

class ComShooting : public Component
{
public:
	ComShooting(CString szName);
	~ComShooting();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void CreateMissile(Vector3 * v, float fSpeed, float fMaxRange);
	void RemoveMissile(GameObject* pMissile);
	void CleanUp();

	static int MissileCount() { return m_vecMissile.size(); }

private:
	// 미사일을 발사한 객체가 파괴 되었을 경우 미사일은 재사용 하기 위해 static으로 선언하는게 중요하다.
	static std::vector<GameObject*> m_vecMissile;	// 발사한 미사일들
	static int MissileIndex;						// 미사일 번호
};

