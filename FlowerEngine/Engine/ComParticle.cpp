#include "stdafx.h"
#include "ComParticle.h"


ComParticle::ComParticle(CString szName) : 
	Component(szName),
	Radius(0.5f),
	numParticle(50),
	texture(NULL)
{
	pDevice9 = GetD3D9Device();
}


ComParticle::~ComParticle()
{
	SAFE_RELEASE(m_VB);
}

void ComParticle::Awake()
{
	m_vecParticles.resize(numParticle);

	for (int i = 0; i < numParticle; ++i)
	{
		Attribute* att = new Attribute();
		att->position.y = 0;
		att->velocity.x = GetRandomFloat(-0.01f, 0.01f);
		att->velocity.z = GetRandomFloat(0.01f, 0.02f);
		att->velocity.y = GetRandomFloat(-0.01f, 0.01f);
		// 불은 처음에 노란색 이후 빨간색
		att->color = 0xFFFFFF99;	// 노란색 R - 255	G - 255		B - 153
		//att->color = 0xFFFF0000;	// 빨간색 R - 255	G - 0		B - 0
		att->age = i * 0.1f;
		m_vecParticles[i] = att;
	}

	pDevice9->CreateVertexBuffer(numParticle * sizeof(Particle),
		D3DUSAGE_POINTS | D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		Particle::FVF, D3DPOOL_DEFAULT, &m_VB, 0);

	texture = Assets::GetTexture(L"Resources/particle/snow.png");

	gameObject->transform->SetPosition(0, 0, -1);
}

void ComParticle::Update()
{
	UpdateFire();
	//TeacherUpdate();
	VBLock();
}

void ComParticle::UpdateFire()
{
	for (size_t i = 0; i < m_vecParticles.size(); ++i)
	{
		Attribute* att = m_vecParticles[i];
		att->age += CClock::GetInstance()->GetFrameDuration();
		att->position.x += att->velocity.x;
		att->position.z -= att->velocity.z;
		att->position.y += att->velocity.y;
		//att->color.r += 0.01f;
		att->color.g -= 0.01f;
		att->color.b -= 0.01f;

		if (att->position.x > 1.0f || att->position.x < -1.0f)
			InitParticle(att);
		if (att->position.z > 2.0f || att->position.z < -2.0f)
			InitParticle(att);
		if (att->position.y > 2.0f)
			InitParticle(att);

		//if (att->color.g <= 0)
		//	att->color.g = 1.0f;
		//if (att->color.b <= 0)
		//	att->color.b = 153 / 255.0f;
	}
}

void ComParticle::TeacherUpdate()
{
	for (size_t i = 0; i < m_vecParticles.size(); ++i)
	{
		Attribute* att = m_vecParticles[i];
		att->age += CClock::GetInstance()->GetFrameDuration();
		att->position.x = cosf(3 * att->age) * Radius;
		att->position.z = sinf(3 * att->age) * Radius;
		att->position.y -= att->velocity.y;

		if (att->position.y < -2.0f)
			att->position.y = 2.0f;
	}
}

void ComParticle::InitParticle(Attribute* pAtt)
{
	pAtt->position.x = 0.f;
	pAtt->position.y = 0.f;
	pAtt->position.z = 0.f;
	pAtt->color.g = 1.0f;
	pAtt->color.b = 153 / 255.0f;
}

void ComParticle::VBLock()
{
	Particle* particle;
	m_VB->Lock(0, 0, (LPVOID*)&particle, D3DLOCK_DISCARD);
	for (auto p : m_vecParticles)
	{
		particle->position = p->position;
		particle->color = p->color;
		++particle;
	}
	m_VB->Unlock();
}

void ComParticle::Render()
{
	pDevice9->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice9->SetRenderState(D3DRS_LIGHTING, false);
	pDevice9->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	pDevice9->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	pDevice9->SetRenderState(D3DRS_POINTSIZE, FtoDw(0.6f));
	pDevice9->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	pDevice9->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	pDevice9->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	pDevice9->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	//pDevice9->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//pDevice9->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//pDevice9->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//pDevice9->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 알파 블랜딩
	pDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 렌더링
	pDevice9->SetTransform(D3DTS_WORLD, &gameObject->transform->GetWorldMatrix());
	pDevice9->SetTexture(0, texture);
	pDevice9->SetFVF(Particle::FVF);
	pDevice9->SetStreamSource(0, m_VB, 0, sizeof(Particle));

	// POINTLIST로 그려줌, 빌보딩이 자동적으로 되고 있음
	pDevice9->DrawPrimitive(D3DPT_POINTLIST, 0, m_vecParticles.size());

	// post
	pDevice9->SetRenderState(D3DRS_ZWRITEENABLE, true);
	pDevice9->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	pDevice9->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	pDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	// restore
	pDevice9->SetRenderState(D3DRS_LIGHTING, true);
	pDevice9->SetTexture(0, NULL);
}

