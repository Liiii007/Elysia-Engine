#include <stdafx.h>
#include "ComponentBase.h"
#include "../World/Entity.h"

ComponentBase::ComponentBase(Entity* entity) {
	parentEntity = entity;
}

std::unordered_map<std::string, std::function<void(Entity&, const rapidjson::Value&)>> ComponentBase::initList;
