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

	// ù��°(root)�� ���� ���� ��
	for (int i = 1; i < iSize; ++i)
	{
		// ���� ������ �� ����
		int c = i;

		do {
			// ���� ���� ��(root)
			int root = (c - 1) / 2;
			if (m_vecData[root] < m_vecData[c])
			{
				// ��ȯ
				int temp = m_vecData[root];
				m_vecData[root] = m_vecData[c];
				m_vecData[c] = temp;
			}
			c = root;
		} while (c != 0);
	}

	// ũ�⸦ �ٿ����� �ݺ������� ���� ����
	for (int i = iSize - 1; i >= 0; --i)
	{
		int temp = m_vecData[0];
		m_vecData[0] = m_vecData[i];
		m_vecData[i] = temp;
		int root = 0;
		int c = 1;
		// �ڽ� �߿� �� ū ���� ã��
		do {
			c = 2 * root + 1;

			if (c < i - 1 && m_vecData[c] < m_vecData[c + 1])
				++c;
			// ��Ʈ ���� �ڽ��� ũ�ٸ� ��ȯ
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
	// ��� ���
	for (int i = 0; i < GetSize(); ++i)
	{
		printf("%d\r\n", m_vecData[i]);
	}
}
