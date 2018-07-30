/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
�� �ڵ�� ���� �����ϰ� �ۼ��� �ڵ��Դϴ�. �̷��� GAME PROGRAMMING GEMS 6���̳� ���ͳ��� �����ϼ���.

���� :
�������(Component)�� ��� Ŭ�����Դϴ�.
*/
#pragma once
class GameObject;
class ComCollider;

class Component
{
public:
	Component(CString szName);
	virtual ~Component(void);

	CString Name() const;

	virtual void Awake() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	
	// �ϴ� �������� �Լ��� ������ �ʰ�
	virtual void ResetDevice(){}

	// �ٸ� �ݶ��̴��� �浹���� �� ȣ��Ǵ� �Լ�
	virtual void OnTriggerEnter(ComCollider &collider) {}

	// �ϴ� public

public:
	static std::vector<ComCollider*> m_vecColliders;
	bool IsAwaked;
	GameObject* gameObject;

protected:
	const CString m_szName;
};
