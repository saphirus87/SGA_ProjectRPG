#include "stdafx.h"
#include "Application/ChrState.h"
#include "Application/IChrState.h"
#include "Application/ChrStateStand.h"
#include "Application/ChrStateWalk.h"
#include "Application/ChrStateAttack.h"

ChrState::ChrState(ComRenderSkinnedMesh* pAnimation)
{
	m_pChrState = new ChrStateStand(pAnimation);
}

ChrState::~ChrState()
{
	SAFE_DELETE(this->m_pChrState);
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

void ChrState::Attack()
{
	m_pChrState->Attack(this);
}
