#include <stdafx.h>
#include <Interface/ICommand.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#define KeyCodeDefine(Key) Key=#@Key

export module InputSystem;

import DXDeviceResource;

export enum Key {
	KeyCodeDefine(A),
	KeyCodeDefine(B),
	KeyCodeDefine(C),
	KeyCodeDefine(D),
	KeyCodeDefine(E),
	KeyCodeDefine(F),
	KeyCodeDefine(G),
	KeyCodeDefine(H),
	KeyCodeDefine(I),
	KeyCodeDefine(J),
	KeyCodeDefine(K),
	KeyCodeDefine(L),
	KeyCodeDefine(M),
	KeyCodeDefine(N),
	KeyCodeDefine(O),
	KeyCodeDefine(P),
	KeyCodeDefine(Q),
	KeyCodeDefine(R),
	KeyCodeDefine(S),
	KeyCodeDefine(T),
	KeyCodeDefine(U),
	KeyCodeDefine(V),
	KeyCodeDefine(W),
	KeyCodeDefine(X),
	KeyCodeDefine(Y),
	KeyCodeDefine(Z),
};

namespace InputSystem {

	bool mWindowMaximized{ false };
	bool mWindowMinimized{ false };
	bool mWindowResizing{ false };
	std::unordered_map<int, std::shared_ptr<ICommand<bool>>> KeyCode{};

	void NewKeyCode(int keycode) {
		KeyCode[keycode] = std::make_shared<ICommand<bool>>(keycode);
	}

	export bool Init() {
		NewKeyCode(Key::A);
		NewKeyCode(Key::S);
		NewKeyCode(Key::D);
		NewKeyCode(Key::W);
		NewKeyCode(Key::B);
		NewKeyCode(Key::Q);
		NewKeyCode(Key::E);
		return true;
	}

	export bool GetKey(int keycode) {
		return KeyCode[keycode]->value;
	}

	export LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return true;

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
			DX::mClientWidth = LOWORD(lParam);
			DX::mClientHeight = HIWORD(lParam);
			if (DX::md3dDevice)
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
					DX::OnResize();
				}
				else if (wParam == SIZE_RESTORED)
				{

					// Restoring from minimized state?
					if (mWindowMinimized)
					{
						//mAppPaused = false;
						mWindowMinimized = false;
						DX::OnResize();
					}

					// Restoring from maximized state?
					else if (mWindowMaximized)
					{
						//mAppPaused = false;
						mWindowMaximized = false;
						DX::OnResize();
					}
					else if (mWindowResizing)
					{
						//DX::OnResize();
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
						DX::OnResize();
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
			DX::OnResize();
			return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

}