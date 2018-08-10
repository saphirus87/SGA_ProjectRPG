#include "stdafx.h"
#include "ChrStateStand.h"
#include "ChrState.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"

ChrStateStand::ChrStateStand(ComRenderSkinnedMesh* pAnimation)
{
	m_pAnimation = pAnimation;
}

ChrStateStand::~ChrStateStand()
{
}

void ChrStateStand::Stand(ChrState * pChrState)
{
	/*CString strDebug("Stand 상태");
	strDebug.Append(L"\r\n");
	OutputDebugString(strDebug);*/

	m_pAnimation->PlayAnimation(eAni_Stand);
}

void ChrStateStand::Walk(ChrState * pChrState)
{
	/*CString strDebug("Stand -> Walk 상태 변환");
	strDebug.Append(L"\r\n");
	OutputDebugString(strDebug);*/

	ChrStateWalk* pStateWalk = new ChrStateWalk(m_pAnimation);
	pChrState->SetState(pStateWalk);
	pStateWalk->Attack(pChrState);
}

void ChrStateStand::Attack(ChrState * pChrState)
{
	/*CString strDebug("Stand -> Attack 상태 변환");
	strDebug.Append(L"\r\n");
	OutputDebugString(strDebug);*/

	ChrStateAttack* pStateAttack = new ChrStateAttack(m_pAnimation);
	pChrState->SetState(pStateAttack);
	pStateAttack->Attack(pChrState);
}
