#pragma once

#include <vector>
#include <memory>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Tools/Logger.h"

#include <DirectXColors.h>
#include <wrl.h>
#include <d3d12.h>
#include <unordered_map>

using UINT = unsigned int;

using Microsoft::WRL::ComPtr;

class MeshRenderer;
class Entity;

struct Vertex1 {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 Texcoord;
};

class MeshData
{
public:
	MeshData(std::string meshPath);

	std::vector<Vertex1> vertices;
	std::vector<uint16_t> indices;

	//Getter for member varient
	std::vector<Vertex1>* getVertices();
	std::vector<uint16_t>* getIndices();
	D3D12_VERTEX_BUFFER_VIEW* VertexBufferView();
	D3D12_INDEX_BUFFER_VIEW* IndexBufferView();

	//Load and store vertex&index to vertices(vector)&indices(vector)
	bool LoadFromDisk(std::string meshPath);

	//Upload Vertices to GPU, called by GriseoRenderer
	void UploadVertices();

	ComPtr<ID3DBlob> VertexBufferCPU;
	ComPtr<ID3DBlob> IndexBufferCPU;
	ComPtr<ID3D12Resource> VertexBufferGPU;
	ComPtr<ID3D12Resource> VertexBufferUploader;
	ComPtr<ID3D12Resource> IndexBufferGPU;
	ComPtr<ID3D12Resource> IndexBufferUploader;
	D3D12_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	std::string path;
	UINT mObjectIndex;
	static std::unordered_map<std::string, MeshData*> meshs;

private:
	D3D12_VERTEX_BUFFER_VIEW mVBV;
	D3D12_INDEX_BUFFER_VIEW mIBV;

};


