#pragma once

class MeshRenderer {
public:

	static std::vector<Mesh*>* getMeshList() {
		static std::vector<Mesh*> renderList;
		return &renderList;
	}

};

