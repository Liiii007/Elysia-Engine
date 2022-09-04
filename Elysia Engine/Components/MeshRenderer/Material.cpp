#include "Material.h"
#include <Renderer/Shader.h>
#include <World/Entity.h>

ReflectBindComponent(Material);

Material::Material(Entity* entity) : ComponentBase(entity) {

}

void Material::Parse(Entity& entity, const rapidjson::Value& parm) {
	std::string shaderName = parm["Shader"].GetString();
	std::string materialName = parm["Name"].GetString();
	entity.AppendComponent<Material>()
		.GetComponent<Material>()
		->Init(materialName, shaderName);
}

void Material::Init(std::string name, std::string shader) {
	this->shader = Shader::instances[shader];

	if (MaterialData::materialDatas.contains(name)) {
		this->data = MaterialData::materialDatas[name];
	}
	else {
		Log::Error("MaterialData not found");
		//Fallback to init material;

		return;
	}
	
	enabled = true;
}

void Material::DrawEditorUI() {
	ImGui::Text(componentName.c_str());
	ImGui::Spacing();
}

std::shared_ptr<Shader> Material::getShader() {
	return shader;
}

void Material::SetShader(const std::string name) {
	shader = Shader::instances[name];
}


