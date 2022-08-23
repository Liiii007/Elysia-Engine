#include "InputSystem.h"
#include <Renderer/imgui/imgui.h>
#include <Renderer/imgui/imgui_impl_win32.h>
#include <Renderer/imgui/imgui_impl_dx12.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool InputSystem::init() {
	return true;
}

LRESULT InputSystem::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;
	return DefWindowProc(hwnd, msg, wParam, lParam);
}