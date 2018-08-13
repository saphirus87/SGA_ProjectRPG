#include "stdafx.h"
#include "ChrStateAttack.h"
#include "ChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"

ChrStateAttack::ChrStateAttack(ComRenderSkinnedMesh* pAnimation)
{
	m_pAnimation = pAnimation;
}

ChrStateAttack::~ChrStateAttack()
{
}

void ChrStateAttack::Stand(ChrState * pChrState)
{
	/*CString strDebug("Attack -> Stand 상태 변환");
	strDebug.Append(L"\r\n");
	OutputDebugString(strDebug);*/

	ChrStateStand* pStateStand = new ChrStateStand(m_pAnimation);
	pChrState->SetState(pStateStand);
	pStateStand->Stand(pChrState);
}

void ChrStateAttack::Walk(ChrState * pChrState)
{
	/*CString strDebug("Attack -> Walk 상태 변환");
	strDebug.Append(L"\r\n");
	OutputDebugString(strDebug);*/

	ChrStateWalk* pStateWalk = new ChrStateWalk(m_pAnimation);
	pChrState->SetState(pStateWalk);
	pStateWalk->Stand(pChrState);
}

void ChrStateAttack::Attack(ChrState * pChrState)
{
	/*CString strDebug("Attack 상태");
	strDebug.Append(L"\r\n");
	OutputDebugString(strDebug);*/

	m_pAnimation->PlayAnimation(eAni_Attack_1);
}
