#include "Mesh.h"
#include "../../System/MeshRenderer.h"

std::vector<float>* Mesh::getVertices() {
	return &vertices;
}

std::vector<uint16_t>* Mesh::getIndices() {
	return &indices;
}

Mesh::Mesh() {
	MeshRenderer::getMeshList()->emplace_back(this);
}

void Mesh::SetView() {
	mVBV.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	mVBV.StrideInBytes = sizeof(Vertex);
	mVBV.SizeInBytes = sizeof(Vertex) * vertices.size();

	mIBV.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	mIBV.Format = DXGI_FORMAT_R16_UINT;
	mIBV.SizeInBytes = sizeof(UINT) * indices.size();
}