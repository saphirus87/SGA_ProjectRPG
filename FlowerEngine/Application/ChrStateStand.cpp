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
	//Stand ����
}

void ChrStateStand::Walk(ChrState * pChrState)
{
	//Stand -> Walk ���º�ȯ
	m_pChrState->SetState(new ChrStateWalk());
}
