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
#include <unordered_map>
#include "../Tools/Common/d3dUtil.h"
#include <unordered_map>

using Microsoft::WRL::ComPtr;

struct PassConstants {
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMFLOAT4X4 viewProj;
};

class Shader {

public:

	ComPtr<ID3DBlob> mvsByteCode;
	ComPtr<ID3DBlob> mpsByteCode;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	Shader(const std::wstring& filename, const std::string& name) :filename(filename), name(std::move(name)) {
		CompileShaders();
		shaders[name] = this;
	}

	static std::unordered_map<std::string, Shader*> shaders;

	std::string name;
	ComPtr<ID3D12PipelineState> mPSO;
	
	ComPtr<ID3D12RootSignature> mRootSignature;


	void CompileShaders() {
		HRESULT hr = S_OK;

		mvsByteCode = CompileShader(nullptr, "VS", "vs_5_0");
		mpsByteCode = CompileShader(nullptr, "PS", "ps_5_0");

		mInputLayout.clear();
		mInputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	}

	void BuildPSO() {

	}

	ComPtr<ID3DBlob> GetVertexShader() {
		return mvsByteCode;
	}

	ComPtr<ID3DBlob> GetPixelShader() {
		return mpsByteCode;
	}

	auto& GetInputLayout() {
		return mInputLayout;
	}

	ComPtr<ID3D12PipelineState> getPSO() {
		return mPSO;
	}

	static ComPtr<ID3D12PipelineState> getInitPSO() {
		return shaders["shader1"]->getPSO();
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

