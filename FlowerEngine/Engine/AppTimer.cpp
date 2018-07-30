#include "StdAfx.h"
#include "AppTimer.h"
#include "AppClock.h"

CTimer::CTimer(CClock* clock)
:Clock(clock)
,CurrentTime(0)
,FrameTime(0)
,bPaused(true)
,Scale(1.0f)
{
	Clock->AddObserver(this);
}

CTimer::~CTimer(void)
{
	Clock->RemoveObserver(this);
}

void CTimer::Notify()
{
	if (!bPaused)
	{
		FrameTime = Clock->GetFrameDuration() * Scale;
		CurrentTime += FrameTime;
	}
}

void CTimer::Start()
{
	bPaused = false;
}

void CTimer::Reset()
{
	CurrentTime = 0;
	FrameTime = 0;
}

void CTimer::Pause(bool bOn)
{
	bPaused = bOn;
}

void CTimer::SetScale(float scale)
{
	Scale = scale;
}

bool CTimer::IsPaused() const
{
	return bPaused;
}

float CTimer::GetScale() const
{
	return Scale;
}
