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

	virtual void Stand(ComChrControl* pChrState) = 0;
	virtual void Walk(ComChrControl* pChrState) = 0;
	virtual void Attack1(ComChrControl* pChrState) = 0;
	virtual void Attack2(ComChrControl* pChrState) = 0;
	virtual void Attack3(ComChrControl* pChrState) = 0;
};
