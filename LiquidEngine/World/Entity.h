#pragma once
#include "../Components/Translation.h"
#include "../Components/MeshRenderer/Mesh.h"

class Entity
{
public:
	Entity(std::string name);
	~Entity();

	bool loadMesh(std::string meshPath);


	void setLocation(float x, float y, float z) {
		translation.position = XMVECTOR{ x,y,z };
	}

	void setRotation(float x, float y, float z) {
		translation.rotation = XMVECTOR{ x,y,z };
	}

	void setScale(float scale) {
		translation.scale = XMVECTOR{ scale,scale,scale };
	}

	void setScale(float x, float y, float z) {
		translation.scale = XMVECTOR{ x,y,z };
	}

	Translation translation;
	std::unique_ptr<Mesh> mesh{nullptr};
	
	std::string name;
};

