#include <stdafx.h>
#include <Interface/ICommand.h>
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
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


	void NewKeyCode(int keycode) {
		Device::KeyCode[keycode] = std::make_shared<ICommand<bool>>(keycode);
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
		return Device::KeyCode[keycode]->value;
	}

	

}