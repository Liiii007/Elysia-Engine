#include <stdafx.h>
#include "ResourceManager.h"
#include <Renderer/Shader.h>
#include <Tools/Logger.h>
#include <Resources/MaterialData.h>
#include <filesystem>


void ResourceManager::Init() {
	Shader::New(L"D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Renderer\\Shaders\\normal.hlsl", "initShader");
	Log::Info("ResourceManager Init");


	//Get all material in directory
	std::filesystem::path matPath = "D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Materials";
	for (const auto& entry : std::filesystem::directory_iterator(matPath)) {
		auto path = entry.path();
		MaterialData::Load(path);
	}


}