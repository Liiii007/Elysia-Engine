#include "Entity.h"



bool Entity::loadMesh(std::string meshPath) {
	auto mesh = getComponent<Mesh>();
	if (mesh == nullptr) {
		Log::Error("δ�ҵ�Mesh���");
		return false;
	}
	else {
		mesh->Init(meshPath);
		mesh->translation = &translation;
		return true;
	}
	
}