#pragma once
#include "timesource.h"

class CTimeSourceWin32 : public CTimeSource
{
public:
	CTimeSourceWin32(void)
	{
		LARGE_INTEGER freq; // Áøµ¿¼ö
		QueryPerformanceFrequency(&freq);
		SecsPerTick = 1.0f / freq.QuadPart;
	}
	~CTimeSourceWin32(void);

	inline virtual double GetTime() const
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return time.QuadPart * SecsPerTick;
	}

private:
	double SecsPerTick;
};
