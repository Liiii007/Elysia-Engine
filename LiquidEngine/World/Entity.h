#pragma once
#include "../Components/Translation.h"
#include "../Components/MeshRenderer/Mesh.h"

class Entity
{
public:
	Translation translation;
	Mesh mesh{&translation};
	
	std::string name;
};

