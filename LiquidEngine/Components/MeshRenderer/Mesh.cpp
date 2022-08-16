#include "Mesh.h"
#include "../../System/MeshRenderer.h"

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
	//BUG:未能抹除当前物体
	for (auto it = MeshRenderer::getMeshList()->begin(); it != MeshRenderer::getMeshList()->end(); it++) {
		if (*it == this) {
			MeshRenderer::getMeshList()->erase(it);
			break;
		}
	}
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
	mIBV.SizeInBytes = sizeof(UINT) * indices.size();
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