#include "Entity.h"

Entity::Entity(std::string name) {
	this->name = name;
}

Entity::~Entity() {

}

bool Entity::loadMesh(std::string meshPath) {
	auto mesh = getComponent<Mesh>();
	if (mesh == nullptr) {
		Log::Error("Î´ÕÒµ½Mesh×é¼þ");
		return false;
	}
	else {
		mesh->Init(meshPath);
		mesh->translation = &translation;
		return true;
	}
	
}