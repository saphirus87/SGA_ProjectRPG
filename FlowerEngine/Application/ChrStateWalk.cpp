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
	// Walk -> Stand 상태 변환
	m_pChrState->SetState(new ChrStateStand(m_pAnimation));
}

void ChrStateWalk::Walk(ChrState * pChrState)
{
	// Walk 상태
	m_pAnimation->PlayAnimation(eAni_Walk);
}
