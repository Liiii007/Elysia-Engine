#pragma once

#include<vector>
#include<memory>
#include"../Components/MeshRenderer/Mesh.h"
//#include"../Components/MeshRenderer/Material.h"

class MeshRenderer
{
public:

	static std::vector<Mesh*>* getMeshList() {
		static std::vector<Mesh*> renderList;
		return &renderList;
	}

	
};

