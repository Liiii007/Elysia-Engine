#include "Material.h"
#include "../../Renderer/Shader.h"
#include "../../World/Entity.h"

std::string Material::componentName = "Material";

Material::Material(Entity* entity) : ComponentBase(entity) {

}

Shader* Material::getShader() {
	return Shader::shaders[shaderName];
}

void Material::setShader(const std::string name) {
	shaderName = name;
}