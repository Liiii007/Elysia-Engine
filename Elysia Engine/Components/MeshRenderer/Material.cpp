#include "Material.h"
#include "../../Renderer/Shader.h"
#include "../../World/Entity.h"

std::string Material::componentName = "Material";
int Material::materialCount{0};
std::unordered_map<std::string, Material*> Material::materials{};

Material::Material(Entity* entity) : ComponentBase(entity) {
	matCBIndex = materialCount;
	materialCount++;
}

void Material::Bind() {
	ComponentBase::initList[Material::componentName] = &Parse;
}

void Material::Parse(Entity& entity, const rapidjson::Value& parm) {
	std::string shaderName = parm["Shader"].GetString();
	std::string materialName = parm["Name"].GetString();
	entity.AppendComponent<Material>()
		.GetComponent<Material>()
		->Init(materialName, shaderName);
}

void Material::Init(std::string name, std::string shader) {
	this->name = name;
	this->shader = Shader::shaders[shader];
	enabled = true;
	materials[name] = this;
}

void Material::DrawEditorUI() {
	ImGui::Text(componentName.c_str());
	ImGui::Spacing();
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