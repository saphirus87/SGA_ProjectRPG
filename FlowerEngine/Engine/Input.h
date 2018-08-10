#pragma once
#include "stdafx.h"

class Input
{
public:

	static Input* GetInstance();

	void Update();

	static bool KeyDown(DWORD key) { return m_pKeyboard->KeyDown(key); }
	static bool KeyUp(DWORD key) { return m_pKeyboard->KeyUp(key); }
	static bool KeyPress(DWORD key) { return m_pKeyboard->KeyPress(key); }

	static bool ButtonDown(DWORD button) { return m_pMouse->ButtonDown(button); }
	static bool ButtonUp(DWORD button) { return m_pMouse->ButtonUp(button); }
	static bool ButtonPress(DWORD button) { return m_pMouse->ButtonPress(button); }
	static bool ButtonDoubleClick(DWORD button) { return m_pMouse->ButtonDB(button); }

	static Vector3 GetMousePosition() { return m_pMouse->GetPosition(); }
	static Vector3 GetMouseDeltaPosition() { return m_pMouse->GetDeltaPosition(); }
	
	// 일단 public으로 풀어두자
	static Mouse* m_pMouse;
	static Keyboard* m_pKeyboard;

private:
	Input();
	~Input();
	
	static Input* m_pInstance;
};

