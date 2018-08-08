#pragma once

class StateStand;
class Character;

class State
{
public:
	State();
	virtual ~State();

	virtual void Stand(Character* character) = 0;
	virtual void Walk(Character* character) = 0;
};

class Character
{
private:
	State * stateClock;

public:
	Character();
	~Character();

	void SetState(State* state);
	void Stand();
	void Walk();
};

class StateStand : public State
{
private:
	Character * character;

public:
	StateStand();
	~StateStand();

	void Stand(Character* character) override;
	void Walk(Character* character) override;
};

class StateWalk : public State
{
private:
	Character * character;

public:
	StateWalk();
	~StateWalk();

	void Stand(Character* character) override;
	void Walk(Character* character) override;
};
