#pragma once

#include "../Tools/Common/d3dUtil.h"
using Microsoft::WRL::ComPtr;

class Shader {
public:
	static std::unordered_map<std::string, std::shared_ptr<Shader>> instances;
	std::string name;
	ComPtr<ID3D12PipelineState> mPSO;
	ComPtr<ID3D12RootSignature> mRootSignature;
	ComPtr<ID3DBlob> mvsByteCode;
	ComPtr<ID3DBlob> mpsByteCode;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	Shader(const std::filesystem::path filename, const std::string name) : filename(filename), name(name) {

	}

	static void New(const std::wstring& filename, const std::string name) {
		instances[name] = std::make_shared<Shader>(filename, name);
	}

	void Build();
	void SetInputLayout();
	void BuildPSO();
	void BuildRootSig();

	void Use(int objectIndex);

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

	bool operator==(const Shader& other) {
		if (this->filename == other.filename) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator!=(const Shader& other) {
		return !(*this == other);
	}

private:
	std::filesystem::path filename;
	ComPtr<ID3DBlob> CompileShader(const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);

};

