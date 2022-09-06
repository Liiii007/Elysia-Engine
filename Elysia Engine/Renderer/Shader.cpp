#include <stdafx.h>
#include "Shader.h"
#include "../Renderer/GriseoRenderer.h"
import DXDeviceResource;

std::unordered_map<std::string, std::shared_ptr<Shader>> Shader::instances{};

ComPtr<ID3DBlob> Shader::CompileShader(const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target) {
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

void Shader::BuildRootSig() {
	const int bCount = 3;
	auto renderer = Singleton<GriseoRenderer>::Get();

	CD3DX12_ROOT_PARAMETER slotRootParameter[bCount];

	//define constant buffer layout
	CD3DX12_DESCRIPTOR_RANGE cbvTable_b0;
	cbvTable_b0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	CD3DX12_DESCRIPTOR_RANGE cbvTable_b1;
	cbvTable_b1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	CD3DX12_DESCRIPTOR_RANGE cbvTable_b2;
	cbvTable_b2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);

	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable_b0);
	slotRootParameter[1].InitAsDescriptorTable(1, &cbvTable_b1);
	slotRootParameter[2].InitAsDescriptorTable(1, &cbvTable_b2);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(bCount, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr) {
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(DX::md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)));

	//require for a bind for command list
}

void Shader::BuildPSO() {
	auto renderer = Singleton<GriseoRenderer>::Get();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	//Needed change
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
		mvsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
		mpsByteCode->GetBufferSize()
	};

	//Set by default
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DX::mBackBufferFormat;
	psoDesc.SampleDesc.Count = DX::m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = DX::m4xMsaaState ? (DX::m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = DX::mDepthStencilFormat;

	ThrowIfFailed(DX::md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}

void Shader::SetInputLayout() {
	mInputLayout.clear();
	mInputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	mInputLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	mInputLayout.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
}

void Shader::Build() {
	mvsByteCode = CompileShader(nullptr, "VS", "vs_5_0");
	mpsByteCode = CompileShader(nullptr, "PS", "ps_5_0");

	SetInputLayout();
	BuildRootSig();
	BuildPSO();
}