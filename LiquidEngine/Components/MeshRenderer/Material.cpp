#include "Material.h"
#include "../../Renderer/Shader.h"
#include "../../World/Entity.h"

std::string Material::componentName = "Material";

Material::Material(Entity* entity) : ComponentBase(entity) {

}

Shader* Material::getShader() {
	return shader;
}

void Material::SetShader(const std::string name) {
	shader = Shader::shaders[name];
}

void Material::SetShader(Shader* shader) {
	this->shader = shader;
}