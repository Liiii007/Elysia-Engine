#include <stdafx.h>

import Shader;
import MaterialData;
import Log;

export module ResourceManager;

namespace ResourceManager {
	using namespace Resource;

	export void Init() {
		Shader::New(L"D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Shaders\\normal.hlsl", "initShader");
		Log::Info("ResourceManager Init");

		//Get all material in directory
		std::filesystem::path matPath = "D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Materials";
		for (const auto& entry : std::filesystem::directory_iterator(matPath)) {
			auto path = entry.path();
			MaterialData::Load(path);
		}

	}
}

