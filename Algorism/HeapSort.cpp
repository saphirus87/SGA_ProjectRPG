#include "stdafx.h"
#include "HeapSort.h"


HeapSort::HeapSort()
{
}


HeapSort::~HeapSort()
{
}

void HeapSort::AddData(int iData)
{
	m_vecData.push_back(iData);
}

void HeapSort::Sort()
{
	int iSize = GetSize();

	// 첫번째(root)는 가장 작은 값
	for (int i = 1; i < iSize; ++i)
	{
		// 다음 값부터 비교 시작
		int c = i;

		do {
			// 가장 작은 값(root)
			int root = (c - 1) / 2;
			if (m_vecData[root] < m_vecData[c])
			{
				// 교환
				int temp = m_vecData[root];
				m_vecData[root] = m_vecData[c];
				m_vecData[c] = temp;
			}
			c = root;
		} while (c != 0);
	}

	// 크기를 줄여가며 반복적으로 힙을 구성
	for (int i = iSize - 1; i >= 0; --i)
	{
		int temp = m_vecData[0];
		m_vecData[0] = m_vecData[i];
		m_vecData[i] = temp;
		int root = 0;
		int c = 1;
		// 자식 중에 더 큰 값을 찾기
		do {
			c = 2 * root + 1;

			if (c < i - 1 && m_vecData[c] < m_vecData[c + 1])
				++c;
			// 루트 보다 자식이 크다면 교환
			if (c < i && m_vecData[root] < m_vecData[c])
			{
				temp = m_vecData[root];
				m_vecData[root] = m_vecData[c];
				m_vecData[c] = temp;
			}
			root = c;
		} while (c < i);
	}
}

void HeapSort::PrintResult()
{
	// 결과 출력
	for (int i = 0; i < GetSize(); ++i)
	{
		printf("%d\r\n", m_vecData[i]);
	}
}
