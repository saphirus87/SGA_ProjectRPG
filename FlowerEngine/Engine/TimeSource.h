#pragma once

class CTimeSource
{
public:
	CTimeSource(void){}
	~CTimeSource(void){}

	virtual double GetTime() const = 0;
};
