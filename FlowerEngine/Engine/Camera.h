#pragma once
class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Delete();
	void Update(float fElapsedTime);
	void Render();

	void SetProj(float fDistZ);

	void SetTarget(Vector3* pTarget) { m_pTarget = pTarget; }
	const Vector3& GetPosition() { return m_eye; }
	const Vector4& GetPosition4() { return Vector4(m_eye, 1); }
	Vector4 GetDOF();
	const Matrix4x4& GetViewMatrix() { return m_matView; }
	const Matrix4x4& GetProjMatrix() { return m_matProj; }
	Matrix4x4 GetVPmatrix() { return (m_matView * m_matProj); }

	static Camera* GetInstance();

	// 절두체(Frustum) 안에 있으면 true 반환, 밖에 있으면 false 반환합니다.
	bool FrustumCulling(Vector3* vPos, float fRadius = 0.f);
	Matrix4x4 m_matWorld;

	// 카메라 업데이트 할지 여부
	bool bUpdate;

private:
	void UpdateFrustum();

private:
	Device9 pDevice9;
	static Camera* m_pInstance;

	vector<D3DXVECTOR3> m_projVtx;	// 원본 정점들
	vector<VERTEX_PC> m_worldVtx;	// 월드 변환 정점들
	vector<D3DXPLANE> m_planes;

	Vector3 m_eye;
	Vector3 m_lookat;
	Vector3 m_up;
	Matrix4x4 m_matView;
	Matrix4x4 m_matProj;

	float m_basePosY;
	float m_distance; // 타겟과의 거리
	float m_rotX;
	float m_rotY;
	float m_currRotX;
	float m_currRotY;
	Vector3* m_pTarget;
};

