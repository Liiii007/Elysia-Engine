#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "../DataStructures.h"

using Microsoft::WRL::ComPtr;

class Model
{
public:
	Model(std::string name, std::string path) :name(name) {
		importFromDisk(path);
	
	};

	std::string name;
	std::vector<Vertex1> vertices;
	std::vector<UINT> indices;
	
	void processMesh(aiMesh* mesh, const aiScene* scene);
	bool importFromDisk(std::string path);

	//Vertex Buffer
	D3D12_VERTEX_BUFFER_VIEW mVBV;
	D3D12_INDEX_BUFFER_VIEW mIBV;

	ComPtr<ID3D12Resource> VertexBufferGPU;
	ComPtr<ID3D12Resource> VertexBufferUploader;
	ComPtr<ID3D12Resource> IndexBufferGPU;
	ComPtr<ID3D12Resource> IndexBufferUploader;

};