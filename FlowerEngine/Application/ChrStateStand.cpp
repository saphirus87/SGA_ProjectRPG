#include "stdafx.h"
#include "ChrStateStand.h"
#include "ChrState.h"
#include "ChrStateWalk.h"

ChrStateStand::ChrStateStand()
{
}

ChrStateStand::~ChrStateStand()
{
}

void ChrStateStand::Stand(ChrState * pChrState)
{
	//Stand 상태
}

void ChrStateStand::Walk(ChrState * pChrState)
{
	//Stand -> Walk 상태변환
	m_pChrState->SetState(new ChrStateWalk());
}
