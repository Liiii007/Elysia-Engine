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

using Microsoft::WRL::ComPtr;

class Shader {
public:
	static std::unordered_map<std::string, Shader*> shaders;
	std::string name;
	ComPtr<ID3D12PipelineState> mPSO;
	ComPtr<ID3D12RootSignature> mRootSignature;
	ComPtr<ID3DBlob> mvsByteCode;
	ComPtr<ID3DBlob> mpsByteCode;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	Shader(const std::wstring& filename, const std::string name) :filename(filename), name(name) {
		shaders[name] = this;
	}

	void Build();
	void SetInputLayout();
	void BuildPSO();
	void BuildRootSig();

	void Use(int);

	ComPtr<ID3DBlob> GetVertexShader() {
		return mvsByteCode;
	}

	ComPtr<ID3DBlob> GetPixelShader() {
		return mpsByteCode;
	}

	auto GetInputLayout() {
		return mInputLayout;
	}

	ComPtr<ID3D12PipelineState> getPSO() {
		return mPSO;
	}

	static ComPtr<ID3D12PipelineState> getInitPSO() {
		return shaders["shader1"]->getPSO();
	}

	


private:
	std::wstring filename;
	ComPtr<ID3DBlob> CompileShader(const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);

};

