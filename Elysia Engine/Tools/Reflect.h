#pragma once
#include <Components/FullComponentHeader.h>
class Reflect
{
public:
	static void Init() {
		Mesh::Bind();
		Material::Bind();
		Light::Bind();
	}
};

