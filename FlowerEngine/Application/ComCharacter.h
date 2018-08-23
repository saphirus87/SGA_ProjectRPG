#pragma once
#include "stdafx.h"
#include "StatusInfo.h"

class AttackHandler : public ID3DXAnimationCallbackHandler
{
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};

class ComChrEquipment;

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

	// Key Frame Event�� �ֱ� ���� �ִϸ��̼��� �����ϰ� ���� ���(Register)�մϴ�. 
	void AnimationCompress();

	// Ÿ���� �����մϴ�.
	void AttackTarget(ComCharacter* pTarget);
	
	// �� �մϴ�.
	void Defence(int dmg);
	
	// ������ �˻��մϴ�.
	bool CheckDeath();

	// UI ����
	virtual void UpdateHPBar() {}

protected:
	void Init();

protected:
	// ĳ���� Ÿ��
	eChrType m_eType;

	ComRenderSkinnedMesh * m_pAnimation;
	
	// �ִϸ��̼�
	vector<LPD3DXKEYFRAMEDANIMATIONSET> m_vecKeyFrameAnimSet;

	// ������ Ÿ��
	ComCharacter* m_pAttackTarget;

	// �ɷ�ġ
	StatusInfo m_status;

	// ��� ����
	ComChrEquipment* m_pChrEquipment;
};