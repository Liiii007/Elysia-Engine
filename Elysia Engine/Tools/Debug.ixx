#include <stdafx.h>
export module Debug;

namespace Debug {
	export bool isDebug{false};
	export void Debug(std::string message) {
		auto s = message;
	}
}