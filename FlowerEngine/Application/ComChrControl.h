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
	bool CheckPickingMon();
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
	// ������ ����� ã���ϴ�.
	void FindAttackTarget();

public:
	// �� ��ü�� ��ŷ�Ǿ����� ����
	bool IsPicking;
	ComRenderSkinnedMesh * pAnimation;
	// �����ϰ��� �ϴ� Ÿ��
	ComCharacter* pAttackTarget;
	ComCharacter* m_pCharacter;
	// ���µ�
	vector<IChrState*> m_vecState;
	// ���� ����
	IChrState * m_pCurrentState;

protected:
	ComObjMap * m_pMap;
	// Ÿ������ ����
	ComFollowTarget* m_pFollow;
	// �̵��ϰ��� �ϴ� ��ġ
	Vector3 vMoveToPoint;
	// Ư�� ��ǥ�� �̵� ����
	bool IsMoveToPoint;
	// �ʱ� ĳ���� ���ý� �� ���� �ִ��� ����
	bool IsGroud;
};