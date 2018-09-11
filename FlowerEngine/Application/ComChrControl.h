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

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
	
private:
	// �� ��ü�� ��ŷ�Ǿ����� ���θ� �˻��մϴ�. (ĳ����, ����, ��)
	void CheckPickingChr();
	void CheckPickingMap();
	// ���� Ŭ���ϸ� �ش� ��ġ�� �̵��մϴ�.
	void MoveToPoint();
	
private:
	// �� ��ü�� ��ŷ�Ǿ����� ����
	bool IsPicking;
	ComCharacter* m_pCharacter;
	ComObjMap * m_pMap;
	// �̵��ϰ��� �ϴ� ��ġ
	Vector3 vMoveToPoint;
	// Ư�� ��ǥ�� �̵� ����
	bool IsMoveToPoint;
};