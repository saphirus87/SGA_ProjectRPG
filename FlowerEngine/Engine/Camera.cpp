#include "stdafx.h"
#include "Camera.h"

Camera* Camera::m_pInstance = NULL;


Camera::Camera() :
	pDevice9(NULL),
	m_pTarget(NULL),
	m_distance(10),
	m_basePosY(3),
	m_rotX(0),
	m_rotY(0),
	m_currRotX(0),
	m_currRotY(0),
	bUpdate(true)
{
	pDevice9 = GetD3D9Device();
	D3DXMatrixIdentity(&m_matWorld);
}

Camera::~Camera()
{
}

void Camera::Init()
{
	m_eye = Vector3(0, m_basePosY, -m_distance);
	m_lookat = Vector3(0, 0, 0);
	m_up = Vector3(0, 1, 0);

	// �޼� ��ǥ�� 
	D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_lookat, &m_up);

	// View�� ��� ���ڴ�.
	pDevice9->SetTransform(D3DTS_VIEW, &m_matView);

	SetProj(100);
	
	pDevice9->SetTransform(D3DTS_PROJECTION, &m_matProj);

	// Projection ��ȯ ���¿����� �������� �ڽ�
	m_projVtx.push_back(D3DXVECTOR3(-1, 1, 1));		// �»���
	m_projVtx.push_back(D3DXVECTOR3( 1, 1, 1));		// �����
	m_projVtx.push_back(D3DXVECTOR3(-1, 1, 0));		// �»���
	m_projVtx.push_back(D3DXVECTOR3( 1, 1, 0));		// �����
	m_projVtx.push_back(D3DXVECTOR3(-1,-1, 1));		// ������
	m_projVtx.push_back(D3DXVECTOR3( 1,-1, 1));		// ������
	m_projVtx.push_back(D3DXVECTOR3(-1,-1, 0));		// ������
	m_projVtx.push_back(D3DXVECTOR3( 1,-1, 0));		// ������

	for (size_t i = 0; i < m_worldVtx.size(); i++)
		m_worldVtx[i].c = WHITE;

	m_worldVtx.resize(8);
	m_planes.resize(6);
}

void Camera::Delete()
{
	SAFE_DELETE(m_pInstance);
}

void Camera::Update(float fElapsedTime)
{
	if (bUpdate == false)
		return;

	// ���콺�� ������ �ǰԲ�
	if (Input::ButtonPress(VK_LBUTTON))
	{
		m_rotY += (Input::GetMouseDeltaPosition().x) / 100.0f;
		m_rotX += (Input::GetMouseDeltaPosition().y) / 100.0f;
		// X�� ����
		if (m_rotX <= -D3DX_PI * 0.4f)
			m_rotX = -D3DX_PI * 0.4f;
		if (m_rotX >= D3DX_PI * 0.35f)
			m_rotX = D3DX_PI * 0.35f;
	}

	// z���� ��
	m_distance -= Input::GetMouseDeltaPosition().z / 120.0f;
	// 2�̻� 100����
	m_distance = max(2, m_distance);
	m_distance = min(100, m_distance);

	m_eye = Vector3(0, m_basePosY, -m_distance);

	// ȸ��
	Matrix4x4 matRotX, matRotY, matRot;
	D3DXMatrixRotationX(&matRotX, m_rotX);
	D3DXMatrixRotationY(&matRotY, m_rotY);
	matRot = matRotX * matRotY;

	// 1�ʵ��� 10��ŭ �����̰ڴ� deltaTime
	// ȸ�� ����
	EaseTo(&m_currRotX, m_currRotX, m_rotX, 10.0f * fElapsedTime);
	EaseTo(&m_currRotY, m_currRotY, m_rotY, 10.0f * fElapsedTime);
	D3DXMatrixRotationX(&matRotX, m_currRotX);
	D3DXMatrixRotationY(&matRotY, m_currRotY);
	matRot = matRotX * matRotY;

	D3DXVec3TransformCoord(&m_eye, &m_eye, &matRot);

	Matrix4x4 matT;
	D3DXMatrixTranslation(&matT, m_eye.x, m_eye.y, m_eye.z);
	m_matWorld = matT * matRot;

	if (m_pTarget)
	{
		// Ÿ���� ������ Ÿ���� �ٶ󺸰Բ�
		m_lookat = *m_pTarget;
		// �� ��ġ�� Ÿ���� ��������
		m_eye = *m_pTarget + m_eye;
	}

	// �� �缳��
	D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_lookat, &m_up);

	pDevice9->SetTransform(D3DTS_VIEW, &m_matView);
	
	UpdateFrustum();

}

Vector4 Camera::GetDOF()
{
	//return Vector4(m_eye, -0.6f);

	Vector3 vLength = m_eye - *m_pTarget;
	float fDist = D3DXVec3Length(&vLength);
	Vector4 vfocal = Vector4(0, 0, 0, -0.6f);;
	vfocal.w = fDist - 3.6f; // alpha?
	return vfocal;
}

void Camera::Render()
{
	// Grid�� ���ܷ� LIGHTING�� ����
	/*pDevice9->SetRenderState(D3DRS_LIGHTING, false);
	pDevice9->SetFVF(VERTEX_PC::FVF);
	pDevice9->DrawPrimitiveUP(D3DPT_LINELIST, m_worldVtx.size() / 2, &m_worldVtx[0], sizeof(VERTEX_PC));
	pDevice9->SetRenderState(D3DRS_LIGHTING, true);*/
}

void Camera::SetProj(float fDistZ)
{
	RECT clientRect = DXUTGetWindowClientRect();
	// ���� ��� �޼� ��ǥ��, �þ߰� 45��, ȭ�� ������ ��Ⱦ��, ���̴� �Ÿ� 1~1000
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, clientRect.right / (float)clientRect.bottom, 1, fDistZ);
}

Camera * Camera::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new Camera();
		m_pInstance->Init();
	}

	return m_pInstance;
}

bool Camera::FrustumCulling(Vector3 * vPos, float fRadius)
{
	for (auto p : m_planes)
	{
		if (D3DXPlaneDotCoord(&p, vPos) - fRadius > 0)
		{
			return false;
		}
	}

	return true;
}

void Camera::UpdateFrustum()
{
	D3DXMATRIXA16 matProj, matView;
	pDevice9->GetTransform(D3DTS_PROJECTION, &matProj);
	pDevice9->GetTransform(D3DTS_VIEW, &matView);

	for (size_t i = 0; i < m_worldVtx.size(); i++)
	{
		D3DXVec3Unproject(&m_worldVtx[i].p, &m_projVtx[i], NULL, &matProj, &matView, NULL);
	}

	//�����
	D3DXPlaneFromPoints(&m_planes[0], &m_worldVtx[2].p, &m_worldVtx[3].p, &m_worldVtx[6].p);
	//�����
	D3DXPlaneFromPoints(&m_planes[1], &m_worldVtx[1].p, &m_worldVtx[0].p, &m_worldVtx[5].p);
	//�����
	D3DXPlaneFromPoints(&m_planes[2], &m_worldVtx[0].p, &m_worldVtx[2].p, &m_worldVtx[4].p);
	//�����
	D3DXPlaneFromPoints(&m_planes[3], &m_worldVtx[3].p, &m_worldVtx[1].p, &m_worldVtx[7].p);
	//�����
	D3DXPlaneFromPoints(&m_planes[4], &m_worldVtx[0].p, &m_worldVtx[1].p, &m_worldVtx[2].p);
	//�����
	D3DXPlaneFromPoints(&m_planes[5], &m_worldVtx[6].p, &m_worldVtx[7].p, &m_worldVtx[4].p);
}
