#include "stdafx.h"
#include "Application/ChrState.h"
#include "Application/IChrState.h"
#include "Application/ChrStateStand.h"
#include "Application/ChrStateWalk.h"

ChrState::ChrState()
{
	m_pChrState = new ChrStateStand();
}

ChrState::~ChrState()
{
	SAFE_DELETE(m_pChrState);
}

void ChrState::SetState(IChrState * pChrState)
{
	delete this->m_pChrState;
	this->m_pChrState = pChrState;
}

void ChrState::Stand()
{
	m_pChrState->Stand(this);
}

void ChrState::Walk()
{
	m_pChrState->Walk(this);
}
