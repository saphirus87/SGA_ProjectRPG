#pragma once
#include "stdafx.h"

// 파티클 렌더용
struct Particle
{
	Vector3 position;
	D3DCOLOR color;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

// 파티클 속성값
struct Attribute
{
	Attribute()
	{
		position = Vector3(0, 0, 0);
		color = 0xFFFFFFFF;
	}
	~Attribute() {}
	Vector3 position;	// position 값을 velocity 만큼 움직인다든지..
	Color color;		// 프레임당 움직이는 가속도
	Vector3 velocity;	// 파티클 색상값
	float age;			// 현재 프레임에서 얼마나 지났나
};

class ComParticle : public Component
{
public:
	ComParticle(CString szName);
	~ComParticle();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 생성된 파티클 전체 갯수
	size_t numParticle;
	// 파티클 텍스쳐
	Texture texture;
	// 반지름 값
	float Radius;

	void UpdateFire(); // 일단 과제용
	void TeacherUpdate();

private:
	void InitParticle(Attribute* pAtt);
	void VBLock();

private:
	Device9 pDevice9;
	VertexBuffer m_VB;
	vector<Attribute*> m_vecParticles;
};

