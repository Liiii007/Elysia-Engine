#include "WorldManager.h"

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
	//FIXED
	JSONHandler::save(path, config);
	return true;
}

void WorldManager::changeItem(std::string name, int value) {
	Value& s = config[name.c_str()];
	s.SetInt(value);
	save(outputPath);
}

int WorldManager::GetInt(std::string name) {
	return config[name.c_str()].GetInt();
}