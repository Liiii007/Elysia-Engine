#include "InputSystem.h"

bool InputSystem::init() {
	return true;
}

LRESULT InputSystem::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}