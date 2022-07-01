#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

using UINT = unsigned int;


struct TEXCOORD {
	float x, y;
};

struct VERTEX {
	float x, y, z;
};

struct Texture {
	std::string type;
	std::string path;
	UINT texture;

	void Release() {
		//SafeRelease(texture);
	}
};


class Model
{
public:
	Model(std::string name) :name(name) {};

	std::string name;

	std::vector<VERTEX> vertices;
	std::vector<VERTEX> normals;
	std::vector<TEXCOORD> texcoords;
	std::vector<UINT> indices;
	
	void processMesh(aiMesh* mesh, const aiScene* scene);
	bool importFromDisk(std::string path);


};