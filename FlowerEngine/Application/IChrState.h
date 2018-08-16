/*
김동오
E-Mail	: eden1521@naver.com

설명 :
캐릭터 상태 기계 클래스 입니다.
*/
#pragma once

class ComChrControl;

class IChrState
{
public:
	IChrState(){}
	virtual ~IChrState(){}

	virtual void Stand(ComChrControl* pChrState) = 0;
	virtual void Walk(ComChrControl* pChrState) = 0;
	virtual void Attack1(ComChrControl* pChrState) = 0;
	virtual void Attack2(ComChrControl* pChrState) = 0;
	virtual void Attack3(ComChrControl* pChrState) = 0;
};
