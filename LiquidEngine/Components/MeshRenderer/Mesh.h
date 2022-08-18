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
#include "../../Tools/Logger.h"

#include <DirectXColors.h>
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <rttr/type.h>


using UINT = unsigned int;

using Microsoft::WRL::ComPtr;

class MeshRenderer;

class Vertex {
	DirectX::XMFLOAT3 Position;

};

class Mesh
{
public:
	Mesh(std::string meshPath);
	~Mesh();

	Translation* translation;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<uint16_t> indices;

	std::vector<float>* getVertices();
	std::vector<uint16_t>* getIndices();
	XMMATRIX getWorldMatrix();
	D3D12_VERTEX_BUFFER_VIEW* VertexBufferView();
	D3D12_INDEX_BUFFER_VIEW Mesh::IndexBufferView()const;

	void SetBufferView();
	void SetToBox();
	bool LoadFromDisk(std::string meshPath);
	void SetTranslation(Translation& t);
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

private:
	D3D12_VERTEX_BUFFER_VIEW mVBV;
	D3D12_INDEX_BUFFER_VIEW mIBV;

};

