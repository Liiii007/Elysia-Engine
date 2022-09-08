#include <stdafx.h>

export module ReflectSystem;

import Material;
import Mesh;
import DirectLight;
import Translation;
import Definition;
import Camera;
import ECS;

#define Bind(T)\
ComponentBase::initList[#T] = T::Parse;\

namespace ReflectSystem {
	export void Init() {
		using namespace Component;
		Bind(Mesh);
		Bind(Material);
		Bind(DirectLight);
		Bind(Translation);
		Bind(Camera);
	}
}
