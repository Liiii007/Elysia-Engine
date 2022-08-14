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
#include "../../Components/Translation.h"

#include <DirectXColors.h>
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>

struct ObjectConstants {
	DirectX::XMFLOAT4X4 worldMatrix;
};

struct PassConstants {
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMFLOAT4X4 viewProj;
};

using UINT = unsigned int;

using Microsoft::WRL::ComPtr;

class MeshRenderer;

class Mesh
{
public:
	Mesh(Translation* ptr);
	Mesh::Mesh(Translation* ptr, std::string meshPath);
	Mesh::~Mesh();

	const Translation* translation;
	std::vector<float> vertices;
	std::vector<uint16_t> indices;

	std::vector<float>* getVertices();
	std::vector<uint16_t>* getIndices();
	ObjectConstants getWorldMatrix();


	void SetBufferView();
	void SetToBox();
	void Mesh::LoadFromDisk();

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

