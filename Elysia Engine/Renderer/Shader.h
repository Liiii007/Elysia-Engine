#pragma once

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


struct ObjectConstants {
	DirectX::XMFLOAT4X4 gWorld;
	DirectX::XMFLOAT4 gColor;
};

struct PassConstants {
	//FIXED:HLSL����4byte�������ζ��ÿ��float3���涼���Զ�����һ��byte�������Ҫ�ֶ�ƥ��padding
	DirectX::XMFLOAT4X4 gViewProj;

	//4Byte
	DirectX::XMFLOAT3 viewPos;
	float pad1;

	//4Byte
	DirectX::XMFLOAT3 viewDir;
	float pad2;

	//4Byte
	DirectX::XMFLOAT3 lightDir;
	float pad3;

	//4Byte
	DirectX::XMFLOAT3 lightPos;
	float pad4;

};

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
	std::wstring filename;
	ComPtr<ID3DBlob> CompileShader(const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);

};

