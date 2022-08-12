#pragma once

#include<vector>
#include<memory>
#include <vector>
#include <array>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "../../DataStructures.h"

using Microsoft::WRL::ComPtr;

class MeshRenderer;

class Mesh
{
public:

	std::vector<float> vertices;
	std::vector<uint16_t> indices;

	Mesh();
	std::vector<float>* getVertices();
	std::vector<uint16_t>* getIndices();
	void SetView();

	//Vertex Buffer
	D3D12_VERTEX_BUFFER_VIEW mVBV;
	D3D12_INDEX_BUFFER_VIEW mIBV;

	ComPtr<ID3DBlob> VertexBufferCPU;
	ComPtr<ID3DBlob> IndexBufferCPU;
	ComPtr<ID3D12Resource> VertexBufferGPU;
	ComPtr<ID3D12Resource> VertexBufferUploader;
	ComPtr<ID3D12Resource> IndexBufferGPU;
	ComPtr<ID3D12Resource> IndexBufferUploader;
};

