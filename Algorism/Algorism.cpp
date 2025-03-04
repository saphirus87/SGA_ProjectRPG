// Algorism.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "HeapSort.h"
#include "StudyState.h"

void RunHeapSort()
{
	HeapSort heapSort;

	heapSort.AddData(19);
	heapSort.AddData(8);
	heapSort.AddData(2);
	heapSort.AddData(1);
	heapSort.AddData(24);
	heapSort.AddData(29);
	heapSort.AddData(3);
	heapSort.AddData(23);
	heapSort.AddData(234);
	heapSort.AddData(32);

	heapSort.Sort();

	heapSort.PrintResult();
}



int main()
{
	//RunHeapSort();
	Character* myClock = new Character();
	myClock->Stand();
	myClock->Walk();
	myClock->Stand();

	//getchar();
	delete myClock;
	system("pause");
    return 0;
}
