#include <stdafx.h>

export module ReflectSystem;

import Material;
import Mesh;
import Light;
import Definition;
import ECS;

#define Bind(T)\
ComponentBase::initList[#T] = T::Parse;\

namespace ReflectSystem {
	export void Init() {
		using namespace Component;
		Bind(Mesh);
		Bind(Material);
		Bind(Light);

	}
}
