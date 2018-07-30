#include "StdAfx.h"
#include "Component.h"

std::vector<ComCollider*> Component::m_vecColliders;

Component::Component(CString szName) :
	gameObject(NULL),
	m_szName(szName),
	IsAwaked(false)
{
}

Component::~Component(void)
{
}

CString Component::Name() const
{
	return m_szName;
}