#include "WorldManager.h"

#include <Components/FullComponentHeader.h>
#include <Renderer/Shader.h>


bool WorldManager::Init(std::filesystem::path levelJsonPath) {
	auto d = JSONHandler::load(levelJsonPath);

	//Confirm file type
	if (!d.HasMember("METADATA") && !d["METADATA"].HasMember("Type") && !(d["METADATA"]["Type"] == "Level")) {
		return false;
	}

	if (!d.HasMember("Entities")) {
		return false;
	}

	const Value& entities = d["Entities"];
	
	//Append every entity and components
	for (auto& entity : entities.GetArray()) {
		if (!entity.HasMember("Name")) {
			Log::Error("Entity struct incorrect");
			continue;
		}

		auto name = entity["Name"].GetString();

		try {
			Entity& e = Entity::New(name);

			//Translation
			{
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
			}

			//Components
			{
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
		catch(std::exception exce) {
			Log::Error("Failed to load entity");
			Entity::Erase(name);
		};
		

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