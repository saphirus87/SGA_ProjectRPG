#pragma once
#include "stdafx.h"

#define MAX_INPUT_MOUSE 8

class Mouse
{
private:
	HWND hWnd;
	Vector3 position; //마우스 스크린 좌표

	byte buttonStatus[MAX_INPUT_MOUSE];
	byte buttonOldStatus[MAX_INPUT_MOUSE];
	byte buttonMap[MAX_INPUT_MOUSE];

	Vector3 currPosition;
	Vector3 oldPosition;
	Vector3 deltaPosition;

	DWORD timeDblClk;
	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];

	enum INPUT_STATUS
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};
		
public:
	Mouse();
	~Mouse();
	void Init(HWND handle);
	void Update();
	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);

	Vector3 GetPosition() { return position; }
	Vector3 GetDeltaPosition() { return deltaPosition; }

	bool ButtonDown(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN;}
	bool ButtonUp(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_UP;}
	bool ButtonPress(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS;}
	bool ButtonDB(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DBLCLK;}

	/*enum INPUT_TYPE
	{
		LBUTTON,
		RBUTTON,
		MBUTTON
	};*/

};

