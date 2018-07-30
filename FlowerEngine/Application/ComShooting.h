#pragma once
#include "stdafx.h"

class ComShooting : public Component
{
public:
	ComShooting(CString szName);
	~ComShooting();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void CreateMissile(Vector3 * v, float fSpeed, float fMaxRange);
	void RemoveMissile(GameObject* pMissile);
	void CleanUp();

	static int MissileCount() { return m_vecMissile.size(); }

private:
	// �̻����� �߻��� ��ü�� �ı� �Ǿ��� ��� �̻����� ���� �ϱ� ���� static���� �����ϴ°� �߿��ϴ�.
	static std::vector<GameObject*> m_vecMissile;	// �߻��� �̻��ϵ�
	static int MissileIndex;						// �̻��� ��ȣ
};

