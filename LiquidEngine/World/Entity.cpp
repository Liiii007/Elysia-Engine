#include "Entity.h"



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