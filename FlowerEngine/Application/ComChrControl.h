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
	~ComChrControl();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetState(IChrState* pChrState);
private:
	void GetHeight();
	void Move();

	//���� ���
	void Stand();
	void Walk(float fDeltaZ);
	void Attack1();
private:
	ComObjMap * m_pMap;
	ComRenderSkinnedMesh * m_pAnimation;
	IChrState * m_pCurrentState;

	Vector3 m_vecForward;			// �� ���� ����

};