#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
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

