#include "stdafx.h"
#include "ComChrControl.h"
#include "ChrStateAttack.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"

ChrStateAttack::ChrStateAttack(ComRenderSkinnedMesh* pAnimation)
{
	m_pAnimation = pAnimation;
	m_pAnimation->PlayAnimation(eAni_Attack_1);
}

ChrStateAttack::~ChrStateAttack()
{
}

void ChrStateAttack::Stand(ComChrControl * pChrState)
{
	/*CString strDebug("Attack -> Stand ���� ��ȯ");
	strDebug.Append(L"\r\n");
	OutputDebugString(strDebug);*/

	//ChrStateStand* pStateStand = new ChrStateStand(m_pAnimation);
	//pChrState->SetState(pStateStand);
	//pStateStand->Stand(pChrState);
}

void ChrStateAttack::Walk(ComChrControl * pChrState)
{
	OutputDebugString(L"Attack -> Walk ���� ��ȯ\r\n");

	ChrStateWalk* pStateWalk = new ChrStateWalk(m_pAnimation);
	pChrState->SetState(pStateWalk);
}

void ChrStateAttack::Attack(ComChrControl * pChrState)
{
	/*CString strDebug("Attack ����");
	strDebug.Append(L"\r\n");
	OutputDebugString(strDebug);*/

	//m_pAnimation->PlayAnimation(eAni_Attack_1);
}
