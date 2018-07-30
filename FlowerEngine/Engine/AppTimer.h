#pragma once
#include "DPObserver.h"

class CClock;

class CTimer : public IObserver
{
public:
	CTimer(CClock* clock);
	~CTimer(void);

	// ��� �Դϴ�.
	void Start();
	void Reset();
	void Pause(bool bOn);

	// ������ �Դϴ�.
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
