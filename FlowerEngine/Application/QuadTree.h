#pragma once
#include "stdafx.h"

class QuadTree
{
	enum CornerType { CORNER_TR, CORNER_TL, CORNER_BR, CORNER_BL };
	enum QuadLocation { FRUSTUM_OUT, FRUSTUM_PARTIALLY_IN, FRUSTUM_COMPLETELY_IN, FRUSTUM_UNKNOWN = -1 };
private:
	QuadTree * m_pChild[4];

	int m_Center;
	int m_Corner[4];

	bool m_Culled;
	float m_Radius;

private:
	vector<VERTEX_PNT>* m_MapVertex;
	vector<DWORD>* m_pIndex;

	UINT m_TotalSizeX;

private:
	// 자식 노드 추가
	QuadTree * _AddChild(int CornerTR, int CornerTL, int CornerBR, int CornerBL);

	// 4개의 코너값 설정
	bool _SetCorners(int CornerTR, int CornerTL, int CornerBR, int CornerBL);

	// 쿼드 트리를 4개의 하위 트리로 분할
	bool _SubDivide();

	bool _IsVisible(int LODLevel) { return (m_Corner[CORNER_TL] - m_Corner[CORNER_TR] <= LODLevel); }

	// 출력할 폴리곤의 인덱스 생성
	int _GenTriIndex(int Triangles, vector<DWORD>& pIndex);

	// 절두체 안에 현재 노드가 포함 되었는지 여부 확인
	int _IsInFrustum();
	// 절두체 컬링
	void _FrustumCull();

	// LOD(Level Of Detail)
	float _GetDistance(Vector3& CenterPos);
	int _GetLODLevel();

	// 쿼드 트리 삭제
	void _Destroy();
public:
	QuadTree(int x, int y);
	QuadTree(QuadTree* pParent);
	~QuadTree();

	// 쿼드트리 초기화
	void Init();

	// 쿼드 트리 구축
	bool Build();

	// 삼각형의 인덱스를 만들고, 출력할 삼각형의 갯수 반환
	int GenerateIndex(vector<DWORD>& pIndex);

	void SetMapVertex(vector<VERTEX_PNT>* vertex) { m_MapVertex = vertex; }
	void SetIndex(vector<DWORD>* index) { m_pIndex = index; }
	//void SetIndex2(vector<DWORD>* index) { m_pIndex2 = index; }
};

