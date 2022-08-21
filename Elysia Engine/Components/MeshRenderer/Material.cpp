#include "Material.h"
#include "../../Renderer/Shader.h"
#include "../../World/Entity.h"

std::string Material::componentName = "Material";

Material::Material(Entity* entity) : ComponentBase(entity) {

}

void Material::Bind() {
	ComponentBase::initList[Material::componentName] = &Parse;
}

void Material::Parse(Entity& entity, const rapidjson::Value& parm) {
	std::string componentInitParm = parm.GetString();
	entity.AppendComponent<Material>()
		.GetComponent<Material>()
		->SetShader(Shader::shaders[componentInitParm]);
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