#include "stdafx.h"
#include "StudyState.h"

Normal::Normal()
{
	printf("Normal ����\n");
}

void Normal::normal(Clock* clock)
{
	printf("normal �Լ�\n");
}
void Normal::alarm(Clock* clock)
{
	printf("Alarm�� �︳�ϴ�.\n");
}

Clock::Clock()
{
	stateClock = new Normal();
}
