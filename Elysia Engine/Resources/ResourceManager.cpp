#include "ResourceManager.h"
#include <Renderer/Shader.h>
#include <Tools/Logger.h>

void ResourceManager::Init() {
	Shader::New(L"D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Renderer\\Shaders\\normal.hlsl", "initShader");
	Log::Info("ResourceManager Init");
}