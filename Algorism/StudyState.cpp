#include "stdafx.h"
#include "StudyState.h"

Normal::Normal()
{
	printf("Normal 상태\n");
}

void Normal::normal(Clock* clock)
{
	printf("normal 함수\n");
}
void Normal::alarm(Clock* clock)
{
	printf("Alarm을 울립니다.\n");
}

Clock::Clock()
{
	stateClock = new Normal();
}
