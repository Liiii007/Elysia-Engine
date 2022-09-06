#include <stdafx.h>
#include <Tools/Common/d3dUtil.h>
#include <Components/Translation.h>

export module Mesh;

import DXDeviceResource;
import ECS;

namespace Component {
	using Microsoft::WRL::ComPtr;
	using namespace DirectX;

	struct Vertex {
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Texcoord;
	};

	export class Mesh;
	export std::vector<Mesh*> meshs;

	export class Mesh : public ComponentBase {
	public:
		Mesh(Entity* entity);
		~Mesh();

		//Parse init data from json file
		static void Parse(Entity& entity, const rapidjson::Value& parm);

		virtual void DrawEditorUI();

		//Actual Init
		Mesh* Init(std::string meshPath);
		Entity& ReturnParentEntity();

		//Note:the translation inherit from parent Entity
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

		//Upload Vertices to GPU, called by GriseoRenderer
		void UploadVertices();

		//DX12 Resources
		ComPtr<ID3DBlob> VertexBufferCPU{ nullptr };
		ComPtr<ID3DBlob> IndexBufferCPU{ nullptr };
		ComPtr<ID3D12Resource> VertexBufferGPU{ nullptr };
		ComPtr<ID3D12Resource> VertexBufferUploader{ nullptr };
		ComPtr<ID3D12Resource> IndexBufferGPU{ nullptr };
		ComPtr<ID3D12Resource> IndexBufferUploader{ nullptr };
		D3D12_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		std::string path{};
		int mObjectIndex{ -1 };

	private:
		D3D12_VERTEX_BUFFER_VIEW mVBV{};
		D3D12_INDEX_BUFFER_VIEW mIBV{};

	};

	std::vector<Vertex>* Mesh::getVertices() {
		return &vertices;
	}

	std::vector<uint16_t>* Mesh::getIndices() {
		return &indices;
	}

	Mesh::Mesh(Entity* entity) :ComponentBase(entity) {

	}

	void Mesh::Parse(Entity& entity, const rapidjson::Value& parm) {
		using namespace rapidjson;
		std::string componentInitParm = parm.GetString();
		entity.AppendComponent<Mesh>()
			.GetComponent<Mesh>()
			->Init(componentInitParm);
	}

	void Mesh::DrawEditorUI() {
		ImGui::Text("Mesh");
		ImGui::Spacing();
	}

	Mesh* Mesh::Init(std::string meshPath) {
		meshs.push_back(this);
		if (LoadFromDisk(meshPath)) {
			mIBV.SizeInBytes = 0;
			mVBV.SizeInBytes = 0;
			return this;
		}
		else {
			Log::Error("Unable to load mesh");
			return this;
		}

	}

	Entity& Mesh::ReturnParentEntity() {
		return *parentEntity;//Define in ComponentBase.h
	}

	Mesh::~Mesh() {

	}

	bool Mesh::LoadFromDisk(std::string meshPath) {

		//Filling
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(meshPath,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		if (scene == nullptr) {
			Log::Error(importer.GetErrorString());
			return false;
		}

		auto mesh = scene->mMeshes[0];
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex v{};
			//push position
			v.Position.x = mesh->mVertices[i].x;
			v.Position.y = mesh->mVertices[i].y;
			v.Position.z = mesh->mVertices[i].z;

			//push normal
			v.Normal.x = mesh->mNormals[i].x;
			v.Normal.y = mesh->mNormals[i].y;
			v.Normal.z = mesh->mNormals[i].z;

			v.Texcoord.x = mesh->mTextureCoords[0][i].x;
			v.Texcoord.y = mesh->mTextureCoords[0][i].y;

			vertices.push_back(v);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}

		return true;
	}



	D3D12_VERTEX_BUFFER_VIEW* Mesh::VertexBufferView()
	{
		if (vertices.size() == 0) {
			Log::Error("Empty vertex buffer");
			return nullptr;
		}

		D3D12_VERTEX_BUFFER_VIEW vbv{};
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = sizeof(vertices[0]);
		vbv.SizeInBytes = sizeof(vertices[0]) * vertices.size();

		mVBV = vbv;

		return &mVBV;
	}

	D3D12_INDEX_BUFFER_VIEW* Mesh::IndexBufferView()
	{
		if (indices.size() == 0) {
			Log::Error("Empty vertex buffer");
			return nullptr;
		}

		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = DXGI_FORMAT_R16_UINT;
		ibv.SizeInBytes = sizeof(indices[0]) * indices.size();

		mIBV = ibv;

		return &mIBV;
	}

	XMMATRIX Mesh::getWorldMatrix() {
		auto r = parentEntity->translation.rotation;
		auto rX = XMMatrixRotationX(r.x / 6.28f);
		auto rY = XMMatrixRotationY(r.y / 6.28f);
		auto rZ = XMMatrixRotationZ(r.z / 6.28f);


		auto rotationMatrix = XMMatrixRotationRollPitchYawFromVector(
			XMVectorSet(
				parentEntity->translation.rotation.x,
				parentEntity->translation.rotation.y,
				parentEntity->translation.rotation.z,
				1
			) / 6.28f);
		auto scaleMatrix = XMMatrixScalingFromVector(
			XMVectorSet(
				parentEntity->translation.scale.x,
				parentEntity->translation.scale.y,
				parentEntity->translation.scale.z,
				1
			));
		auto translationMatrix = XMMatrixTranslationFromVector(
			XMVectorSet(
				parentEntity->translation.position.x,
				parentEntity->translation.position.y,
				parentEntity->translation.position.z,
				1
			));
		return translationMatrix * scaleMatrix * rotationMatrix;
	}

	void Mesh::UploadVertices() {

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(vertices[0]);
		const UINT ibByteSize = (UINT)indices.size() * sizeof(indices[0]);

		ThrowIfFailed(D3DCreateBlob(vbByteSize, &VertexBufferCPU));
		CopyMemory(VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

		ThrowIfFailed(D3DCreateBlob(ibByteSize, &IndexBufferCPU));
		CopyMemory(IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

		VertexBufferGPU = d3dUtil::CreateDefaultBuffer(DX::md3dDevice.Get(),
			DX::mCommandList.Get(), vertices.data(), vbByteSize, VertexBufferUploader);

		IndexBufferGPU = d3dUtil::CreateDefaultBuffer(DX::md3dDevice.Get(),
			DX::mCommandList.Get(), indices.data(), ibByteSize, IndexBufferUploader);
	}

	
}