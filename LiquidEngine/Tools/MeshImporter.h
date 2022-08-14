#pragma once

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <rttr/registration>
#include <iostream>
#include <vector>
#include "../Tools/Logger.h"

class MeshImporter
{
public:
    static bool DoTheImportThing(const std::string& pFile, std::vector<float>& vertices, std::vector<uint16_t>& indices);
};


bool MeshImporter::DoTheImportThing(const std::string& pFile, std::vector<float>& vertices, std::vector<uint16_t>& indices) {

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(pFile,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    // If the import failed, report it
    if (scene == nullptr) {
        Log::Error(importer.GetErrorString());
        return false;
    }

    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}
using namespace rttr;

RTTR_REGISTRATION{
    registration::method("meshImport", &MeshImporter::DoTheImportThing);
}
