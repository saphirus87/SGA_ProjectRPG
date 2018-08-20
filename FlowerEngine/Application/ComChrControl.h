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

	void Init();
	void SetState(int iIndex);
		
	//���� ���
	virtual void Stand();
	virtual void Walk(float fDeltaZ);
	virtual void Attack1();

	// �� ��ü�� ��ŷ�Ǿ����� ���θ� �˻��մϴ�. (ĳ����, ����)
	void CheckPickingChr();
	void CheckPickingMon();

	// �� ��ü�� ��ŷ�Ǿ����� ����
	bool IsPicking;
	ComRenderSkinnedMesh * pAnimation;

protected:
	void GetHeight();
	
protected:
	ComObjMap * m_pMap;
	IChrState * m_pCurrentState;

	Vector3 m_vecForward;			// �� ���� ����

	vector<IChrState*> m_vecState;

	ComFollowTarget* m_pTarget;
	ComCharacter* m_pCharacter;
};