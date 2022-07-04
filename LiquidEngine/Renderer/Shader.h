#pragma once

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include "../Tools/Common/d3dUtil.h"

using Microsoft::WRL::ComPtr;

class Shader {
public:
	Shader(const std::wstring& filename, const std::string name) :filename(filename), name(name) {
		CompileShaders();
	}
	
	std::string name;

	struct InputLayout {

	};

	void CompileShaders() {
		HRESULT hr = S_OK;

		mvsByteCode = CompileShader(nullptr, "VS", "vs_5_0");
		mpsByteCode = CompileShader(nullptr, "PS", "ps_5_0");

		mInputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	}

	ComPtr<ID3DBlob> GetVertexShader() {
		return mvsByteCode;
	}

	ComPtr<ID3DBlob> GetPixelShader() {
		return mpsByteCode;
	}

	auto GetInputLayout() {
		return mInputLayout;
	}

	ComPtr<ID3DBlob> mvsByteCode;
	ComPtr<ID3DBlob> mpsByteCode;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;


private:
	std::wstring filename;
	ComPtr<ID3DBlob> CompileShader(const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target) {
		UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		HRESULT hr = S_OK;

		ComPtr<ID3DBlob> byteCode = nullptr;
		ComPtr<ID3DBlob> errors;
		hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

		if (errors != nullptr)
			OutputDebugStringA((char*)errors->GetBufferPointer());

		ThrowIfFailed(hr);

		return byteCode;
	}

};
