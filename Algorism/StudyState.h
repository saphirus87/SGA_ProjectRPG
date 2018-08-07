#pragma once

class Normal;
class Clock;

class ClockState
{
public:
	ClockState(){}
	virtual ~ClockState() {}

	virtual void normal(Clock* clock) = 0;
	virtual void alarm(Clock* clock) = 0;
};


class Clock
{
private:
	ClockState * stateClock;

public:
	Clock();
	~Clock() {}

	void SetState(ClockState* state) {
		this->stateClock = state;
	}
	void normal() {
		stateClock->normal(this);
	}
	void alarm() {
		stateClock->alarm(this);
	}
};
class Normal : public ClockState
{
private:
	Clock * clock;

public:
	Normal();
	~Normal() {}

	void normal(Clock* clock) override;
	void alarm(Clock* clock) override;
};
