#include "stdafx.h"
#include "StudyState.h"
//----------------State----------------
State::State()
{
	printf("State 생성자 호출됨\n");
}

State::~State()
{
	printf("State 소멸자 호출됨\n");
}
//----------------Character----------------
Character::Character()
{
	stateClock = new StateStand();
}

Character::~Character()
{
	printf("Character 소멸자 호출됨\n");
	if(stateClock!=NULL)
	{
		delete stateClock;
		stateClock = NULL;
	}
}

void Character::SetState(State* state) 
{
	delete this->stateClock;
	this->stateClock = state;
}

void Character::Stand() 
{
	stateClock->Stand(this);
}

void Character::Walk() 
{
	stateClock->Walk(this);
}
//----------------StateStand----------------
StateStand::StateStand()
{
	printf("StateStand 생성자 호출됨\n");
}

StateStand::~StateStand()
{
	printf("StateStand 소멸자 호출됨\n");
}

void StateStand::Stand(Character* character)
{
	printf("Stand 상태\n");
}
void StateStand::Walk(Character* character)
{
	printf("Stand -> Walk상태변환\n");
	character->SetState(new StateWalk());
}
//----------------StateWalk----------------
StateWalk::StateWalk()
{
	printf("StateWalk 생성자 호출됨\n");
}

StateWalk::~StateWalk()
{
	printf("StateWalk 소멸자 호출됨\n");
}

void StateWalk::Stand(Character * character)
{
	printf("Walk -> Stand상태변환\n");
	character->SetState(new StateStand());
}

void StateWalk::Walk(Character * character)
{
	printf("Walk 상태\n");
}
