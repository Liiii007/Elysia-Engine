#include <stdafx.h>
#include "ResourceManager.h"

import Shader;
import MaterialData;
import Log;

using namespace Resource;

void ResourceManager::Init() {
	Shader::New(L"D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Shaders\\normal.hlsl", "initShader");
	Log::Info("ResourceManager Init");


	//Get all material in directory
	std::filesystem::path matPath = "D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Materials";
	for (const auto& entry : std::filesystem::directory_iterator(matPath)) {
		auto path = entry.path();
		MaterialData::Load(path);
	}


}