#include <stdafx.h>

import JSONHandler;
import ECS;
import Log;

using namespace rapidjson;

export module WorldManager;



namespace WorldManager {

	Document config;

	std::string path;
	std::string outputPath{ "Resources\\json\\test1.json" };
	std::vector<Entity> entities;

	bool Init(std::filesystem::path levelJsonPath);

	void appendItem();
	void removeItem();
	void changeItem(std::string name, int value);

	int GetInt(std::string name);

	bool load(std::string path);
	bool save(std::string path);

	//function
	export bool WorldManager::Init(std::filesystem::path levelJsonPath) {
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

			auto entityName = entity["Name"].GetString();

			try {
				//Create New Entity
				Entity& e = Entity::New(entityName);

				//Load Components
				const Value& components = entity["Components"];

				for (auto& component : components.GetArray()) {
					const std::string componentName = component["Type"].GetString();
					const Value& parm = component["Parm"];
					auto deb = ComponentBase::initList;
					if (ComponentBase::initList.find(componentName) == ComponentBase::initList.end()) {
						Log::Error("Not contain required component");
					}
					else {
						ComponentBase::initList[componentName](e, parm);
					}
				}
			}
			catch (std::exception exce) {
				Log::Error("Failed to load entity");
				Entity::Erase(entityName);
			};


		}

		return true;
	}

	void WorldManager::appendItem() {
		//FIXED
	}

	void WorldManager::removeItem() {
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
}


