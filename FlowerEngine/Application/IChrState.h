/*
�赿��
E-Mail	: eden1521@naver.com

���� :
ĳ���� ���� ��� Ŭ���� �Դϴ�.
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
