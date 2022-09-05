#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Interface/ICommand.h>
#include <Windows.h>
#include <unordered_map>

#define BoolKeyCodeInstance(KeyCode)\
std::shared_ptr<ICommand<bool>> KeyCode { std::make_shared<ICommand<bool>>(#@KeyCode)}\

class InputSystem
{
public:
	InputSystem()  {};
	~InputSystem() {};

	bool Init();
	
	static void NewKeyCode(int keycode);

	static bool GetKey(int keycode) {
		return KeyCode[keycode]->value;
	}

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
private:
	static std::unordered_map<int, std::shared_ptr<ICommand<bool>>> KeyCode;

	bool mWindowMaximized{ false };
	bool mWindowMinimized{ false };
	bool mWindowResizing{ false };
};



#define KeyCodeDefine(Key) Key=#@Key

enum Key {
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
