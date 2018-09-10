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

	// �ʱ�ȭ ����
	void Init();
	
	// �� ��ü�� ��ŷ�Ǿ����� ���θ� �˻��մϴ�. (ĳ����, ����, ��)
	void CheckPickingChr();
	void CheckPickingMap();

protected:
	// ���� Ŭ���ϸ� �ش� ��ġ�� �̵��մϴ�.
	void MoveToPoint();
	
public:
	// �� ��ü�� ��ŷ�Ǿ����� ����
	bool IsPicking;
	ComRenderSkinnedMesh * pAnimation;
	ComCharacter* m_pCharacter;
	
protected:
	ComObjMap * m_pMap;
	// �̵��ϰ��� �ϴ� ��ġ
	Vector3 vMoveToPoint;
	// Ư�� ��ǥ�� �̵� ����
	bool IsMoveToPoint;

private:
	// Ÿ������ ����
	ComFollowTarget* m_pFollow;
};