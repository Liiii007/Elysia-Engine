#include "Material.h"
#include "../../Renderer/Shader.h"

std::string Material::componentName = "Material";


Shader* Material::getShader() {
	return Shader::shaders[shaderName];
}

void Material::setShader(const std::string name) {
	shaderName = name;
}