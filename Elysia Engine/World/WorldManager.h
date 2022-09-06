#pragma once
#include <vector>
#include <string>
#include <filesystem>

#include <Components/Translation.h>
#include <Interface/ISerializable.h>
#include <Tools/JSONHandler.h>

class Entity;

class WorldManager : public ISerializable
{
public:
	Document config;

	std::string path;
	std::string outputPath{"Resources\\json\\test1.json"};
;
	std::vector<Entity> entities;

	bool Init(std::filesystem::path levelJsonPath);

	void appendItem();
	void removeItem();
	void changeItem(std::string name, int value);


	int GetInt(std::string name);

	virtual bool load(std::string path);
	virtual bool save(std::string path);
};




