#include "WorldManager.h"

bool WorldManager::Init() {
	//FIXED:º”‘ÿƒ£–Õ
	
	//entities.emplace_back(Entity());
	//entities.emplace_back(Entity());
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