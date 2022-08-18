#pragma once
#include <string>
#include "../../Interface/IComponent.h"

class Shader;

class Material {

public:
	static std::string componentName;
	std::string name;
	std::string shaderName;

	Shader* getShader();
	void setShader(const std::string name);
};

