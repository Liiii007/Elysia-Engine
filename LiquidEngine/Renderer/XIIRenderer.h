#pragma once

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")


using UINT = unsigned int;

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
#include "../Tools/Common/d3dUtil.h"
#include "../Tools/Common/UploadBuffer.h"
#include "../Tools/Singleton.h"
#include "../System/InputSystem.h"
#include "../Renderer/Shader.h"
#include "../System/MeshRenderer.h" 
#include "../Renderer/SphereCamera.h"
#include "../World/Entity.h"
#include <rttr/registration>


using namespace DirectX;
using namespace std;

using Microsoft::WRL::ComPtr;

struct ObjectConstants {
	DirectX::XMFLOAT4X4 gWorld;
};

struct PassConstants {
	DirectX::XMFLOAT4X4 gView;
	DirectX::XMFLOAT4X4 gProj;
	DirectX::XMFLOAT4X4 gViewProj;
};

class XIIRenderer
{
public:

	bool Init(HINSTANCE);

	//Upload
	void UploadVertices(Mesh* mesh);
	void UploadObjectCB(Mesh* mesh);
	void UploadPassCB();

	//Tick
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int RenderTick();
	void Update();

	

public:
	//Init
	bool InitWindow();
	bool InitDirect3D();
	void CreateCommandObjects();
	void CreateSwapChain();
	void CreateDescHeaps();
	void CreatePassConstantBuffer();
	void CreateObjectConstantBuffer(int objectIndex);
	void OnResize();
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	//Render Tick
	
	void ClearForNextFrame();
	void RenderItem(Mesh* mesh, Shader* shader);
	void RenderFrame();
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

	//Command Objects
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

	//Descriptor and view
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap;
	
	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;
	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	//Constant Buffer
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	std::unique_ptr<UploadBuffer<PassConstants>> mPassCB = nullptr;

	//Root Signature
	//ComPtr<ID3D12RootSignature> mRootSignature;

	//PSO
	//ComPtr<ID3D12PipelineState> mPSO;

	XMFLOAT4X4 mProj = MathHelper::Identity4x4();


	POINT mLastMousePos;

	SphereCamera mCamera;

	UINT mRendererItemCount{ 0 };
};

using namespace rttr;
RTTR_REGISTRATION{
	//registration::method("GetDevice", &XIIRenderer::getDevice);
}
