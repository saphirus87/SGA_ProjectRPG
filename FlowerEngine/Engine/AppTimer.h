#pragma once
#include "DPObserver.h"

class CClock;

class CTimer : public IObserver
{
public:
	CTimer(CClock* clock);
	~CTimer(void);

	// 기능 입니다.
	void Start();
	void Reset();
	void Pause(bool bOn);

	// 접근자 입니다.
	void SetScale(float scale);

	double GetTime() const { return CurrentTime; }
	double GetFrameDuration() const { return FrameTime; }
	bool IsPaused() const;
	float GetScale() const;
private:
	virtual void Notify();

	CClock* Clock;
	double CurrentTime;
	double FrameTime;
	bool bPaused;
	float Scale;
};
