#include <stdafx.h>

import DXDeviceResource;
export module ShadowMap;

using namespace Device;

export class ShadowMap final {
public:
	bool isInit{ false };
	int width;
	int height;
	
	ShadowMap();
	void Init(int width, int height);
	void BuildResource();

private:
	D3D12_VIEWPORT mViewport;
	D3D12_RECT mScissorRect;
	DXGI_FORMAT mFormat = DXGI_FORMAT_R24G8_TYPELESS;
	Microsoft::WRL::ComPtr<ID3D12Resource> mShadowMap;
};

ShadowMap::ShadowMap() {

}

void ShadowMap::Init(int width, int height) {
	this->width = width;
	this->height = height;

	mViewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	mScissorRect = { 0, 0, (int)width, (int)height };

	BuildResource();

	isInit = true;
}

void ShadowMap::BuildResource() {
	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = mFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	HRESULT hr = md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&optClear,
		IID_PPV_ARGS(&mShadowMap));


}