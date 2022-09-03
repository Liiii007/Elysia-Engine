#include "ResourceManager.h"
#include <Renderer/Shader.h>
#include <Tools/Logger.h>
#include <Resources/MaterialData.h>

void ResourceManager::Init() {
	Shader::New(L"D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Renderer\\Shaders\\normal.hlsl", "initShader");
	Log::Info("ResourceManager Init");

	//Mat data test
	MaterialData::Load("Green", "D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Materials\\Green.json");
	MaterialData::Load("Red", "D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Materials\\Red.json");
}