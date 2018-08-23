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

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// �ʱ�ȭ ����
	void Init();
		
	//���� ���
	void SetState(int iIndex);
	virtual void Stand();
	virtual void Walk(float fDeltaZ);
	virtual void Attack1();
	virtual void Death();

	// �� ��ü�� ��ŷ�Ǿ����� ���θ� �˻��մϴ�. (ĳ����, ����, ��)
	void CheckPickingChr();
	void CheckPickingMon();
	void CheckPickingMap();

	// ���ݴ���� ����մϴ�.
	void CancleAttackTarget();
	// ����� �ٶ󺾴ϴ�.
	void LookatTarget();
	// ���ݴ���� �׾��� �� ó���� �մϴ�.
	void CheckAttackTargetDeath();

protected:
	// �ʿ��� ���̸� ���ɴϴ�.
	void GetHeight();
	// ���� Ŭ���ϸ� �ش� ��ġ�� �̵��մϴ�.
	void MoveToPoint();

public:
	// �� ��ü�� ��ŷ�Ǿ����� ����
	bool IsPicking;
	ComRenderSkinnedMesh * pAnimation;
	// �����ϰ��� �ϴ� Ÿ��
	ComCharacter* pAttackTarget;

protected:
	ComObjMap * m_pMap;
	// ���� ����
	IChrState * m_pCurrentState;
	// ���µ�
	vector<IChrState*> m_vecState;
	// Ÿ������ ����
	ComFollowTarget* m_pFollow;
	ComCharacter* m_pCharacter;
	// �̵��ϰ��� �ϴ� ��ġ
	Vector3 vMoveToPoint;
	// Ư�� ��ǥ�� �̵� ����
	bool IsMoveToPoint;
};