#pragma once

class IObserver
{
public:
	virtual ~IObserver(void){}
	virtual void Notify() = 0;
};
