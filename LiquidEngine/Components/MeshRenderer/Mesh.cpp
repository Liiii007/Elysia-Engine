#include "Mesh.h"
#include "../../System/MeshRenderer.h"
#include "../../Renderer/XIIRenderer.h"
#include "../../Tools/Singleton.h"
#include "../../World/Entity.h"

std::string Mesh::componentName = "Mesh";

std::vector<Vertex>* Mesh::getVertices() {
	return &vertices;
}

std::vector<uint16_t>* Mesh::getIndices() {
	return &indices;
}

Mesh::Mesh(Entity* entity) :ComponentBase(entity) {
	this->translation = &parentEntity->translation;
}

void Mesh::Init(std::string meshPath) {
	MeshRenderer::getMeshList()->push_back(this);
	if (LoadFromDisk(meshPath)) {
		mIBV.SizeInBytes = 0;
		mVBV.SizeInBytes = 0;
		return;
	}
	else {
		Log::Error("Unable to load mesh");
		return;
	}
	
}

Mesh::~Mesh() {
	//BUG:未能抹除当前物体
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
	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex v;
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

	for (int i = 0; i < mesh->mNumFaces; i++) {
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

	return true;
}

void Mesh::SetTranslation(Translation& t) {
	translation->position = t.position;
	translation->rotation = t.rotation;
	translation->scale    = t.scale;
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
	auto r = translation->rotation;
	auto rX = XMMatrixRotationX(r.x / 6.28f);
	auto rY = XMMatrixRotationY(r.y / 6.28f);
	auto rZ = XMMatrixRotationZ(r.z / 6.28f);
	

	auto rotationMatrix = XMMatrixRotationRollPitchYawFromVector(
		XMVectorSet(
			translation->rotation.x,
			translation->rotation.y,
			translation->rotation.z,
			1
		)/6.28f);
	auto scaleMatrix = XMMatrixScalingFromVector(
		XMVectorSet(
			translation->scale.x,
			translation->scale.y,
			translation->scale.z,
			1
		));
	auto translationMatrix = XMMatrixTranslationFromVector(
		XMVectorSet(
			translation->position.x,
			translation->position.y,
			translation->position.z,
			1
		));
	return translationMatrix * scaleMatrix * rotationMatrix;
}

void Mesh::UploadVertices() {
	auto renderer = Singleton<XIIRenderer>::Get();

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(vertices[0]);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(indices[0]);

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &VertexBufferCPU));
	CopyMemory(VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &IndexBufferCPU));
	CopyMemory(IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	VertexBufferGPU = d3dUtil::CreateDefaultBuffer(renderer->md3dDevice.Get(),
		renderer->mCommandList.Get(), vertices.data(), vbByteSize, VertexBufferUploader);

	IndexBufferGPU = d3dUtil::CreateDefaultBuffer(renderer->md3dDevice.Get(),
		renderer->mCommandList.Get(), indices.data(), ibByteSize, IndexBufferUploader);
}

