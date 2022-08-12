#include "Model.h"

bool Model::importFromDisk(std::string path) {
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
		return false;

	processMesh(*pScene->mMeshes, pScene);

	return true;
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	
	Vertex vertex;

	// Walk through each of the mesh's vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		vertex.Pos.x = mesh->mVertices[i].x;
		vertex.Pos.y = mesh->mVertices[i].y;
		vertex.Pos.z = mesh->mVertices[i].z;

		//vertices.push_back(vertex);
	}

	// Fill the indices
	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			//indices.push_back(face.mIndices[j]);
			;
	}
	//SetView();
}

void Model::SetToBox() {
	//SetView();
}

void Model::SetView() {
	mVBV.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	mVBV.StrideInBytes = sizeof(Vertex);
	mVBV.SizeInBytes = sizeof(Vertex) * vertices.size();

	mIBV.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	mIBV.Format = DXGI_FORMAT_R16_UINT;
	mIBV.SizeInBytes = sizeof(UINT) * indices.size();
}