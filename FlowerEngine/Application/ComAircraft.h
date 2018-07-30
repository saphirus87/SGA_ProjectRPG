#pragma once
#include "stdafx.h"

class ComAircraftControl;
class ComUIPanelDialog;

// This is Status, not Aircraft
class ComAircraft : public Component
{
public:
	ComAircraft(CString szName);
	~ComAircraft();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	inline const int& Level() { return m_iLevel; }
	inline const int& CurrentEXP() { return m_iEXP; }
	inline const int& NextEXP() { return m_vecEXPNext[m_iLevel - 1]; }
	inline const float& MoveSpeed() { return m_fMoveSpeed; }
	float AttackRange();

	void CheckGameEnd();
	bool CheckLevelUp();
	
	// 레벨업
	void LevelUp();
private:
	ComAircraftControl * m_pAircraftControl;
	ComUIText* m_pTextInfo;
	ComUIPanelDialog* m_pPanelUI;

	// Status
	int m_iLevel;
	int m_iEXP;
	float m_fMoveSpeed;
	float m_fAttackRange;
	vector<int> m_vecEXPNext;

	int m_iEnermyCount;			// 적 숫자
};

