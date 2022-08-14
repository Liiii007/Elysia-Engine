#include "Mesh.h"
#include "../../System/MeshRenderer.h"

std::vector<float>& Mesh::getVertices() {
	return vertices;
}

std::vector<uint16_t>& Mesh::getIndices() {
	return indices;
}

Mesh::Mesh(Translation* ptr) {
	MeshRenderer::getMeshList()->push_back(this);
	translation = ptr;
	SetToBox();
}

Mesh::~Mesh() {
	for (auto it = MeshRenderer::getMeshList()->begin(); it != MeshRenderer::getMeshList()->end(); it++) {
		if (*it == this) {
			MeshRenderer::getMeshList()->erase(it);
			break;
		}
	}
	
}

Mesh::Mesh(Translation* ptr, std::string meshPath) {
	MeshRenderer::getMeshList()->emplace_back(this);
	translation = ptr;
	LoadFromDisk();
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

void Mesh::LoadFromDisk() {
	//Filling
}

void Mesh::SetBufferView() {
	mVBV.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	mVBV.StrideInBytes = sizeof(vertices[0]) * 3;
	mVBV.SizeInBytes = sizeof(vertices[0]) * vertices.size();

	mIBV.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	mIBV.Format = DXGI_FORMAT_R16_UINT;
	mIBV.SizeInBytes = sizeof(UINT) * indices.size();
}

ObjectConstants Mesh::getWorldMatrix() {
	auto rotationMatrix = XMMatrixRotationRollPitchYawFromVector(translation->rotation);
	auto scaleMatrix = XMMatrixScalingFromVector(translation->scale);
	auto translationMatrix = XMMatrixTranslationFromVector(translation->position);

	//build the world matrix
	ObjectConstants ocb{};
	XMStoreFloat4x4(&ocb.worldMatrix, translationMatrix * scaleMatrix * rotationMatrix);
	return ocb;
}