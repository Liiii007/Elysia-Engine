#pragma once
#include <unordered_map>
#include <functional>
#include <rapidjson/document.h>

class Entity;

class ComponentBase
{
public:
	ComponentBase(Entity* entity);

	Entity* parentEntity;
	static std::unordered_map<std::string, std::function<void(Entity&, const rapidjson::Value&)>> initList;
};

