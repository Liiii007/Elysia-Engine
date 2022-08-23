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
#include "Components/Translation.h"
#include "Tools/Logger.h"
#include "Interface/ComponentBase.h"

#include <DirectXColors.h>
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>

using UINT = unsigned int;

using Microsoft::WRL::ComPtr;

class MeshRenderer;
class Entity;

struct Vertex {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 Texcoord;
};

class Mesh : public ComponentBase
{
public:
	Mesh(Entity* entity);
	~Mesh();

	//Regist to reflect
	static void Bind();

	//Parse init data from json file
	static void Parse(Entity& entity, const rapidjson::Value& parm);

	virtual void DrawEditorUI();

	//Actual Init
	Mesh* Init(std::string meshPath);
	Entity& ReturnParentEntity();

	//Note:the translation inherit from parent Entity
	Translation* translation;
	std::vector<Vertex> vertices;
	std::vector<float> normals;
	std::vector<uint16_t> indices;

	//Getter for member varient
	std::vector<Vertex>* getVertices();
	std::vector<uint16_t>* getIndices();
	XMMATRIX getWorldMatrix();
	D3D12_VERTEX_BUFFER_VIEW* VertexBufferView();
	D3D12_INDEX_BUFFER_VIEW* IndexBufferView();

	//Load and store vertex&index to vertices(vector)&indices(vector)
	bool LoadFromDisk(std::string meshPath);

	//Upload Vertices to GPU, called by XIIRenderer
	void UploadVertices();

	//DX12 Resources
	ComPtr<ID3DBlob> VertexBufferCPU;
	ComPtr<ID3DBlob> IndexBufferCPU;
	ComPtr<ID3D12Resource> VertexBufferGPU;
	ComPtr<ID3D12Resource> VertexBufferUploader;
	ComPtr<ID3D12Resource> IndexBufferGPU;
	ComPtr<ID3D12Resource> IndexBufferUploader;
	D3D12_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	std::string path;
	UINT mObjectIndex;
	static std::string componentName;

private:
	D3D12_VERTEX_BUFFER_VIEW mVBV;
	D3D12_INDEX_BUFFER_VIEW mIBV;

};

