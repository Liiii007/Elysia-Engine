#include "Material.h"
#include "../../Renderer/Shader.h"

Shader* Material::getShader() {
	return Shader::shaders[shaderName];
}

void Material::setShader(const std::string name) {
	shaderName = name;
}