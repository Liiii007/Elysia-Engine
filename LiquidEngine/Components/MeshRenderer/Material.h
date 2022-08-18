#pragma once
#include <string>
#include "../../Interface/IComponent.h"
#include "../../Interface/ComponentBase.h"

class Shader;
class Entity;

class Material : public ComponentBase {

public:
	Material(Entity* entity);

	static std::string componentName;
	std::string name;
	std::string shaderName;

	Shader* getShader();
	void setShader(const std::string name);
};

