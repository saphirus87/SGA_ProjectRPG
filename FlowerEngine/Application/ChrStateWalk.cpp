#include "stdafx.h"
#include "ChrStateWalk.h"
#include "ChrState.h"
#include "ChrStateStand.h"

ChrStateWalk::ChrStateWalk(ComRenderSkinnedMesh* pAnimation)
{
	m_pAnimation = pAnimation;
}

ChrStateWalk::~ChrStateWalk()
{
	//SAFE_DELETE(m_pChrState);
}

void ChrStateWalk::Stand(ChrState * pChrState)
{
	CString strDebug("Walk -> Stand 상태 변환");
	strDebug.Append(L"\r\n");
	OutputDebugString(strDebug);

	ChrStateStand* pStateStand = new ChrStateStand(m_pAnimation);
	pChrState->SetState(pStateStand);
	pStateStand->Stand(pChrState);
}

void ChrStateWalk::Walk(ChrState * pChrState)
{
	CString strDebug("Walk 상태");
	strDebug.Append(L"\r\n");
	OutputDebugString(strDebug);
	
	m_pAnimation->PlayAnimation(eAni_Walk);
}
