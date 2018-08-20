#include "stdafx.h"
#include "UIControl.h"


UIControl::UIControl()
	: m_pSprite(NULL),
	  m_pParent(NULL),
	  m_Color(1.0f, 1.0f, 1.0f, 1.0f),
	  m_Position(0.0f, 0.0f, 0.0f),
	  m_Pivot(0.0f, 0.0f, 0.0f)
{
}


UIControl::~UIControl()
{
}
