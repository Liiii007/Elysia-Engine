#pragma once
#include "../Interface/ICommand.h"
#include <Windows.h>

class InputSystem
{
public:
	InputSystem()  {};
	~InputSystem() {};

	bool init();

	//ICommand* W;
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

