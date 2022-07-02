#pragma once

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DirectXColors.h>
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include "../Tools/Singleton.h"
#include "../System/InputSystem.h"
#include "../Tools/Common/d3dUtil.h"

using namespace DirectX;
using namespace std;

using Microsoft::WRL::ComPtr;

class XIIRenderer
{
public:
	XIIRenderer() {};
	~XIIRenderer() {};

	bool Init(HINSTANCE);

	int RenderTick();


	//D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS qualityLevels;

private:
	bool InitWindow();
	bool InitDirect3D();
	void CreateSwapChain();
	void CreateRtvAndDsvDescHeap();
	void OnResize();
	void FlushCommandQueue();
	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;


	HINSTANCE mHInstance = nullptr;
	HWND mhMainWnd = nullptr;
	int mClientWidth{ 800 }, mClientHeight{ 600 };
	int mRefreshRate{ 60 };
	std::wstring mMainWndCaption = L"XII Renderer";

	//device
	ComPtr<IDXGIFactory4> mdxgiFactory;
	ComPtr<IDXGISwapChain> mSwapChain;
	ComPtr<ID3D12Device> md3dDevice;
	bool      m4xMsaaState = false;    // 4X MSAA enabled
	UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	//Format
	DXGI_FORMAT mBackBufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
	DXGI_FORMAT mDepthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

	//something about Command
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence{ 0 };

	//Resource and Desc
	const static int mSwapChainBufferCount = 2;
	int mCurrentBackBufferCount = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[mSwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	//Desc and view
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;
	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;


};



