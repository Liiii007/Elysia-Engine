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
	VERTEX vertex;
	TEXCOORD texcoord;
	VERTEX normal;

	// Walk through each of the mesh's vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;
		vertices.push_back(vertex);

		if (mesh->mTextureCoords[0]) {
			texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			texcoord.y = (float)mesh->mTextureCoords[0][i].y;
			texcoords.push_back(texcoord);
		}

		if (mesh->mNormals) {
			normal.x = (float)mesh->mNormals[i].x;
			normal.y = (float)mesh->mNormals[i].y;
			normal.z = (float)mesh->mNormals[i].z;
			normals.push_back(normal);
		}
		
	}

	// Fill the indices
	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

}