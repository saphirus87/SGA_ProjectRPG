#pragma once
#include "stdafx.h"
#include "StatusInfo.h"

class AttackHandler : public ID3DXAnimationCallbackHandler
{
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};

// ĳ������ �������Դϴ�.
class ComCharacter : public Component
{
public:
	ComCharacter(CString szName);
	virtual ~ComCharacter();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// Ÿ���� �����մϴ�.
	void AttackTarget(ComCharacter* pTarget);
	
	// �� �մϴ�.
	void Defence(int dmg);

	// ������ �˻��մϴ�.
	bool CheckDeath();

protected:
	void Init();

protected:
	ComRenderSkinnedMesh * m_pAnimation;
	StatusInfo m_status;
};