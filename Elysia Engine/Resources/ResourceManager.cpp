#include "ResourceManager.h"
#include <Renderer/Shader.h>
#include <Tools/Logger.h>
#include <Resources/MaterialData.h>
#include <filesystem>


void ResourceManager::Init() {
	Shader::New(L"D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Renderer\\Shaders\\normal.hlsl", "initShader");
	Log::Info("ResourceManager Init");

	//Mat data test
	MaterialData::Load("Green", "D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Materials\\Green.json");
	MaterialData::Load("Red", "D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Materials\\Red.json");

	//Get all material in directory
	std::string matPath = "D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Materials";
	for (const auto& entry : std::filesystem::directory_iterator(matPath)) {
		auto p = entry.path();
	}
}