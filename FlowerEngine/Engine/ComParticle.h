#pragma once
#include "stdafx.h"

// ��ƼŬ ������
struct Particle
{
	Vector3 position;
	D3DCOLOR color;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

// ��ƼŬ �Ӽ���
struct Attribute
{
	Attribute()
	{
		position = Vector3(0, 0, 0);
		color = 0xFFFFFFFF;
	}
	~Attribute() {}
	Vector3 position;	// position ���� velocity ��ŭ �����δٵ���..
	Color color;		// �����Ӵ� �����̴� ���ӵ�
	Vector3 velocity;	// ��ƼŬ ����
	float age;			// ���� �����ӿ��� �󸶳� ������
};

class ComParticle : public Component
{
public:
	ComParticle(CString szName);
	~ComParticle();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// ������ ��ƼŬ ��ü ����
	size_t numParticle;
	// ��ƼŬ �ؽ���
	Texture texture;
	// ������ ��
	float Radius;

	void UpdateFire(); // �ϴ� ������
	void TeacherUpdate();

private:
	void InitParticle(Attribute* pAtt);
	void VBLock();

private:
	Device9 pDevice9;
	VertexBuffer m_VB;
	vector<Attribute*> m_vecParticles;
};

