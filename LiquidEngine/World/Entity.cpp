#include "Entity.h"

Entity::Entity(std::string name) {
	this->name = name;
}

bool Entity::loadMesh(std::string meshPath) {
	mesh = std::make_unique<Mesh>(meshPath);
	mesh->translation = &translation;
	return true;
}