#pragma once
#include <string>

class Shader;

class Material
{
public:


	std::string name;
	std::string shaderName;

	Shader* getShader();
	void setShader(const std::string name);
};

