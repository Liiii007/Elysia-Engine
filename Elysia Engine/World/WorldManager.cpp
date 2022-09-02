#include "WorldManager.h"

#include <Components/FullComponentHeader.h>
#include <Renderer/Shader.h>


bool WorldManager::Init() {

	JSONHandler hand{};
	auto d = hand.load("D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Level\\Level1.json");

	if (d.HasMember("Entities")) {

		const Value& entities = d["Entities"];

		for (auto& entity : entities.GetArray()) {

			//Name
			auto name = entity["Name"].GetString();
			Entity::New(name);

			if (Entity::GetEntity(name) == nullptr) {
				Log::Error("Entity not exist");
				continue;
			}

			Entity& e = *(Entity::GetEntity(name));

			//Translation
			const Value& locationValue = entity["Location"];
			const Value& rotationValue = entity["Rotation"];
			const Value& scaleValue = entity["Scale"];
			const XMFLOAT3 location{
				locationValue[0].GetFloat(),
				locationValue[1].GetFloat(),
				locationValue[2].GetFloat()
			};

			const XMFLOAT3 rotation{
				rotationValue[0].GetFloat(),
				rotationValue[1].GetFloat(),
				rotationValue[2].GetFloat()
			};

			const XMFLOAT3 scale{
				scaleValue[0].GetFloat(),
				scaleValue[1].GetFloat(),
				scaleValue[2].GetFloat()
			};
			
			e.SetLocation(location)
			 .SetRotation(rotation)
			 .SetScale(scale);

			//Components
			const Value& components = entity["Components"];

			for (auto& component : components.GetArray()) {
				const std::string componentName = component["Type"].GetString();
				const Value& parm = component["Parm"];

				if (ComponentBase::initList.find(componentName) == ComponentBase::initList.end()) {
					Log::Error("Not contain required component");
				}
				else {
					ComponentBase::initList[componentName](e, parm);
				}
			}
		}
	}

	return true;
}

inline void WorldManager::appendItem() {
	//FIXED
}

inline void WorldManager::removeItem() {
	//FIXED
}

bool WorldManager::load(std::string path) {
	JSONHandler handler;
	config = handler.load(path);

	//check if load success
	if (config.IsNull()) {

		return false;
	}

	return true;
}
bool WorldManager::save(std::string path) {
	JSONHandler handler;

	return handler.save(path, config);;
}

void WorldManager::changeItem(std::string name, int value) {
	Value& s = config[name.c_str()];
	s.SetInt(value);
	save(outputPath);
}

int WorldManager::GetInt(std::string name) {
	return config[name.c_str()].GetInt();
}