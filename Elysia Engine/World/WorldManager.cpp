#include "WorldManager.h"

#include <Components/FullComponentHeader.h>
#include <Renderer/Shader.h>

bool WorldManager::Init() {

	Entity::New("e2")
		.SetLocation(0, 0, 0)
		.SetRotation(-90, 90, 0)
		.AppendComponent<Mesh>()
		.AppendComponent<Material>()
		.GetComponent<Mesh>()
			->Init("D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Model\\dawei.fbx")
			->ReturnParentEntity()
		.GetComponent<Material>()
			->SetShader(Shader::shaders["initShader"])
		;

	Entity::New("eLight")
		.SetLocation(3, 3, 3)
		.AppendComponent<Light>()
		.GetComponent<Light>()
			->SetTarget(XMFLOAT3(0, 0, 0))
			->ReturnParentEntity();

	return true;
}

inline void WorldManager::appendItem() {
	//FIXED
}

inline void WorldManager::removeItem() {
	//FIXED
}

bool WorldManager::load(std::string path) {
	config = JSONHandler::load(path);

	//check if load success
	if (config.IsNull()) {

		return false;
	}

	return true;
}
bool WorldManager::save(std::string path) {
	return JSONHandler::save(path, config);;
}

void WorldManager::changeItem(std::string name, int value) {
	Value& s = config[name.c_str()];
	s.SetInt(value);
	save(outputPath);
}

int WorldManager::GetInt(std::string name) {
	return config[name.c_str()].GetInt();
}