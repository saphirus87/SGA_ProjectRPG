/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
이 코드는 직접 설계하고 작성한 코드입니다. 이론은 GAME PROGRAMMING GEMS 6권이나 인터넷을 참고하세요.

설명 :
구성요소(Component)의 기반 클래스입니다.
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
	
	// 일단 순수가상 함수로 만들지 않고
	virtual void ResetDevice(){}

	// 다른 콜라이더에 충돌했을 때 호출되는 함수
	virtual void OnTriggerEnter(ComCollider &collider) {}

	// 일단 public

public:
	static std::vector<ComCollider*> m_vecColliders;
	bool IsAwaked;
	GameObject* gameObject;

protected:
	const CString m_szName;
};
