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

	virtual void Stand(int iIndex) = 0;
	virtual void Walk(int iIndex) = 0;
	virtual void Attack1(int iIndex) = 0;
	virtual void Death(int iIndex) = 0;
};
