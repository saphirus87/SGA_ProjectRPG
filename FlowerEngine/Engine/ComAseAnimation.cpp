#include "stdafx.h"
#include "ComAseAnimation.h"


ComAseAnimation::ComAseAnimation(CString szName) : 
	Component(szName),
	m_pScene(NULL)
{
}

ComAseAnimation::~ComAseAnimation()
{
	//SAFE_DELETE(m_pScene);
}

void ComAseAnimation::Awake()
{
}

void ComAseAnimation::Update()
{
	int firstTick = m_pScene->numFirstFrame * m_pScene->TicksPerFrame;
	int lastTick = m_pScene->numLastFrame * m_pScene->TicksPerFrame;

	// 프레임당 증가하는
	int tickSpeed = m_pScene->FrameSpeed * m_pScene->TicksPerFrame;

	DWORD tickCount = (int)((tickSpeed / 1000.0f) * GetTickCount()) % (lastTick - firstTick) + firstTick;

	for (auto & ani : vecRenderAse)
		ani->UpdateAniMatrices(tickCount, &gameObject->transform->GetWorldMatrix());
}

void ComAseAnimation::Render()
{
}
