#include <stdafx.h>
export module Definition;

template<typename T>
using Action = std::function<void(T)>;