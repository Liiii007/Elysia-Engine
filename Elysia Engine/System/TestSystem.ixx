#include <stdafx.h>
export module TestSystem;

import InputSystem;

export bool Get() {
	return InputSystem::GetKey('W');
}
export void MyFunc();