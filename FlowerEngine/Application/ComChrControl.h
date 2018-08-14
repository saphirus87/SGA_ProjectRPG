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

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	void Move();

private:
	ComObjMap * m_pMap;

	Vector3* m_pos;					// ĳ���� ��ġ
	Vector3 m_vecForward;			// �� ���� ����
	bool m_isMoving;				// ������ ���濩��
	bool m_isRotating;				// ���� ���濩��

public:
	bool GetMoving(bool isMoving);
};