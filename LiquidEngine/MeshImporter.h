#pragma once

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>
#include <vector>

class MeshImporter
{
public:
    bool DoTheImportThing(const std::string& pFile);
};



bool MeshImporter::DoTheImportThing(const std::string& pFile) {

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(pFile,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    // If the import failed, report it
    if (scene == nullptr) {
        std::cout << importer.GetErrorString();
        return false;
    }

    std::vector<float> vertices;


    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}
