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
	Shader* shader;

	Shader* getShader();
	void SetShader(const std::string name);
	void SetShader(Shader* shader);
};

