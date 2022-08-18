#include "Mesh.h"
#include "../../System/MeshRenderer.h"
#include "../../Renderer/XIIRenderer.h"
#include "../../Tools/Singleton.h"

std::vector<float>* Mesh::getVertices() {
	return &vertices;
}

std::vector<uint16_t>* Mesh::getIndices() {
	return &indices;
}

Mesh::Mesh(std::string meshPath) {
	MeshRenderer::getMeshList()->push_back(this);
	if (LoadFromDisk(meshPath)) {
		return;
	}
	else {
		Log::Error("Unable to load mesh");
		//SetToBox();
		return;
	}
}

Mesh::~Mesh() {
	//BUG:δ��Ĩ����ǰ����
}

void Mesh::SetToBox() {
	vertices = {
		-1.0f, -1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,
		-1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, -1.0f, +1.0f,
	};

	indices = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
}

bool Mesh::LoadFromDisk(std::string meshPath) {

	//Filling
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(meshPath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (scene == nullptr) {
		Log::Error(importer.GetErrorString());
		return false;
	}

	auto mesh = scene->mMeshes[0];
	for (int i = 0; i < mesh->mNumVertices; i++) {
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}
	
	for (int i = 0; i < mesh->mNumVertices; i++) {
		normals.push_back(mesh->mNormals[i].x);
		normals.push_back(mesh->mNormals[i].y);
		normals.push_back(mesh->mNormals[i].z);
	}

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}

void Mesh::SetTranslation(Translation& t) {
	translation->position = t.position;
	translation->rotation = t.rotation;
	translation->scale    = t.scale;
}

void Mesh::SetBufferView() {
	mVBV.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	mVBV.StrideInBytes = sizeof(vertices[0]) * 3;
	mVBV.SizeInBytes = sizeof(vertices[0]) * vertices.size();

	mIBV.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	mIBV.Format = DXGI_FORMAT_R16_UINT;
	mIBV.SizeInBytes = sizeof(indices[0]) * indices.size();
}

D3D12_VERTEX_BUFFER_VIEW* Mesh::VertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW vbv{};
	vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = sizeof(vertices[0]) * 3;
	vbv.SizeInBytes = sizeof(vertices[0]) * vertices.size();

	mVBV = vbv;

	return &mVBV;
}

D3D12_INDEX_BUFFER_VIEW Mesh::IndexBufferView()const
{
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = sizeof(UINT) * indices.size();

	return ibv;
}

XMMATRIX Mesh::getWorldMatrix() {
	auto r = translation->rotation;
	XMFLOAT3 a;
	XMStoreFloat3(&a, r);
	auto rX = XMMatrixRotationX(a.x / 6.28f);
	auto rY = XMMatrixRotationY(a.y / 6.28f);
	auto rZ = XMMatrixRotationZ(a.z / 6.28f);

	auto rotationMatrix = XMMatrixRotationRollPitchYawFromVector(translation->rotation/6.28f);
	auto scaleMatrix = XMMatrixScalingFromVector(translation->scale);
	auto translationMatrix = XMMatrixTranslationFromVector(translation->position);
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

	SetBufferView();
}

