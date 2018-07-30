#pragma once
#include <vector>
using namespace std;

// �� ������ ��ü �ð� ���⵵�� O(N * logN)
class HeapSort
{
public:
	HeapSort();
	~HeapSort();

	void AddData(int iData);
	void Sort();
	void PrintResult();
	inline int GetSize() { return m_vecData.size(); }

private:
	vector<int> m_vecData;
};