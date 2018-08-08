#include "stdafx.h"
#include "StudyState.h"
//----------------State----------------
State::State()
{
	printf("State ������ ȣ���\n");
}

State::~State()
{
	printf("State �Ҹ��� ȣ���\n");
}
//----------------Character----------------
Character::Character()
{
	stateClock = new StateStand();
}

Character::~Character()
{
	printf("Character �Ҹ��� ȣ���\n");
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
	printf("StateStand ������ ȣ���\n");
}

StateStand::~StateStand()
{
	printf("StateStand �Ҹ��� ȣ���\n");
}

void StateStand::Stand(Character* character)
{
	printf("Stand ����\n");
}
void StateStand::Walk(Character* character)
{
	printf("Stand -> Walk���º�ȯ\n");
	character->SetState(new StateWalk());
}
//----------------StateWalk----------------
StateWalk::StateWalk()
{
	printf("StateWalk ������ ȣ���\n");
}

StateWalk::~StateWalk()
{
	printf("StateWalk �Ҹ��� ȣ���\n");
}

void StateWalk::Stand(Character * character)
{
	printf("Walk -> Stand���º�ȯ\n");
	character->SetState(new StateStand());
}

void StateWalk::Walk(Character * character)
{
	printf("Walk ����\n");
}
