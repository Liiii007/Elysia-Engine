#pragma once
#include<functional>

template<typename T>
using Action = std::function<void(T)>;