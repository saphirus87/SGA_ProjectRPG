#include "stdafx.h"
#include "ChrStateWalk.h"
#include "ChrState.h"
#include "ChrStateStand.h"

ChrStateWalk::ChrStateWalk()
{
}

ChrStateWalk::~ChrStateWalk()
{
}

void ChrStateWalk::Stand(ChrState * pChrState)
{
	// Walk -> Stand 상태 변환
	m_pChrState->SetState(new ChrStateStand());
}

void ChrStateWalk::Walk(ChrState * pChrState)
{
	// Walk 상태
}
