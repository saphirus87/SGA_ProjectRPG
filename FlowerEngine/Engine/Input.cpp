#include "stdafx.h"
#include "Input.h"

Mouse* Input::m_pMouse = NULL;
Keyboard* Input::m_pKeyboard = NULL;
Input* Input::m_pInstance = NULL;

Input::Input()
{
	m_pMouse = new Mouse();
	m_pMouse->Init(GetForegroundWindow());
	m_pKeyboard = new Keyboard();
}


Input::~Input()
{
}

Input * Input::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new Input();

	return m_pInstance;
}

void Input::Update()
{
	if (m_pMouse)
		m_pMouse->Update();

	if (m_pKeyboard)
		m_pKeyboard->Update();
}
