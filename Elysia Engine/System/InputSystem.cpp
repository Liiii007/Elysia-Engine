#include "InputSystem.h"
#include <Renderer/imgui/imgui.h>
#include <Renderer/imgui/imgui_impl_win32.h>
#include <Renderer/imgui/imgui_impl_dx12.h>
#include <Tools/Singleton.h>
#include <Renderer/GriseoRenderer.h>

std::unordered_map<int, std::shared_ptr<ICommand<bool>>> InputSystem::KeyCode{};
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool InputSystem::Init() {
	NewKeyCode(Key::A);
	NewKeyCode(Key::S);
	NewKeyCode(Key::D);
	NewKeyCode(Key::W);
	return true;
}

void InputSystem::NewKeyCode(int keycode) {
	KeyCode[keycode] = std::make_shared<ICommand<bool>>(keycode);
}

LRESULT InputSystem::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	auto renderer = Singleton<GriseoRenderer>::Get();

	if (msg == WM_KEYDOWN) {
		if (KeyCode.contains(wParam)) {
			KeyCode[wParam]->OnValueUpdate(true);
		}
	}

	if (msg == WM_KEYUP) {
		if (KeyCode.contains(wParam)) {
			KeyCode[wParam]->OnValueUpdate(false);
		}
	}

	switch (msg) {

	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		renderer->mClientWidth = LOWORD(lParam);
		renderer->mClientHeight = HIWORD(lParam);
		if (renderer->md3dDevice)
		{
			
			if (wParam == SIZE_MINIMIZED)  //Min Window
			{
				//mAppPaused = true;
				//mMinimized = true;
				//mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)//Max Window
			{
				//mAppPaused = false;
				//mMinimized = false;
				//mMaximized = true;
				renderer->OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (mWindowMinimized)
				{
					//mAppPaused = false;
					mWindowMinimized = false;
					renderer->OnResize();
				}

				// Restoring from maximized state?
				else if (mWindowMaximized)
				{
					//mAppPaused = false;
					mWindowMaximized = false;
					renderer->OnResize();
				}
				else if (mWindowResizing)
				{
					//renderer->OnResize();
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					renderer->OnResize();
				}
			}


		}
		return 0;

	case WM_ENTERSIZEMOVE:
		//mAppPaused = true;
		mWindowResizing = true;
		//mTimer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		//mAppPaused = false;
		mWindowResizing = false;
		//mTimer.Start();
		renderer->OnResize();
		return 0;
	}

	
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}