#include "ResourceManager.h"
#include <Renderer/Shader.h>

void ResourceManager::Init() {
	Shader::New(L"D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Renderer\\Shaders\\normal.hlsl", "initShader");
}