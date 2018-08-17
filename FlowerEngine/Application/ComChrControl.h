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

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void Init();
	void SetState(int iIndex);

	ComRenderSkinnedMesh * m_pAnimation;

	//���� ���
	virtual void Stand();
	virtual void Walk(float fDeltaZ);
	virtual void Attack1();

protected:
	void GetHeight();
	
protected:
	ComObjMap * m_pMap;
	IChrState * m_pCurrentState;

	Vector3 m_vecForward;			// �� ���� ����

	vector<IChrState*> m_vecState;
};