#include <stdafx.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#include <Tools/Common/UploadBuffer.h>
#include <Tools/Common/d3dUtil.h>
export module DXDeviceResource;

namespace DX {
	using namespace DirectX;

	//device
	ComPtr<IDXGIFactory4> mdxgiFactory;
	export ComPtr<IDXGISwapChain> mSwapChain;
	export ComPtr<ID3D12Device> md3dDevice;
	export bool      m4xMsaaState = false;    // 4X MSAA enabled
	export UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	//Format
	export DXGI_FORMAT mBackBufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
	export DXGI_FORMAT mDepthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

	//Command Objects
	export ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	export ComPtr<ID3D12CommandQueue> mCommandQueue;
	export ComPtr<ID3D12GraphicsCommandList> mCommandList;
	export ComPtr<ID3D12Fence> mFence;
	export UINT64 mCurrentFence{ 0 };

	//Descriptor and view
	export ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	export ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	export ComPtr<ID3D12DescriptorHeap> mCbvHeap;
	export ComPtr<ID3D12DescriptorHeap> mSrvHeap;

	export UINT mRtvDescriptorSize = 0;
	export UINT mDsvDescriptorSize = 0;
	export UINT mCbvSrvUavDescriptorSize = 0;
	export D3D12_VIEWPORT mScreenViewport;
	export D3D12_RECT mScissorRect;

	export HINSTANCE mHInstance = nullptr;
	export HWND mhMainWnd = nullptr;
	export int mClientWidth{ 1920 }, mClientHeight{ 720 };
	int mRefreshRate{ 60 };

	//Resource and Desc
	export int mSwapChainBufferCount = 2;
	export int mCurrentBackBufferCount = 0;
	export Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[2];
	export Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	//Constant Buffer
	export std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	export std::unique_ptr<UploadBuffer<PassConstants>> mPassCB = nullptr;
	export std::unique_ptr<UploadBuffer<MaterialConstants>> mMaterialCB = nullptr;

	export int mPassCBCount{ 1 };
	export int mMaterialCBCount{ 2 };

	POINT mLastMousePos;

	export int mRendererItemCount{ 0 };
	export int mFrameCount{ 0 };
}

namespace DX {
	void CreateCommandObjects() {
		//Create Command Queue
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

		//Create Command Allocator
		ThrowIfFailed(md3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(mCommandAllocator.GetAddressOf())));

		//Create Command List
		ThrowIfFailed(md3dDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			mCommandAllocator.Get(), // Associated command allocator
			nullptr,                   // Initial PipelineStateObject
			IID_PPV_ARGS(mCommandList.GetAddressOf())));

		// Start off in a closed state.  This is because the first time we refer 
		// to the command list we will Reset it, and it needs to be closed before
		// calling Reset.
		mCommandList->Close();
	}

	void CreateSwapChain() {
		mSwapChain.Reset();

		DXGI_SWAP_CHAIN_DESC sd{};
		sd.BufferDesc.Width = mClientWidth;
		sd.BufferDesc.Height = mClientHeight;
		sd.BufferDesc.RefreshRate.Numerator = mRefreshRate;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = mBackBufferFormat;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
		sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = mSwapChainBufferCount;
		sd.OutputWindow = mhMainWnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Note: Swap chain uses queue to perform flush.
		ThrowIfFailed(mdxgiFactory->CreateSwapChain(
			mCommandQueue.Get(),
			&sd,
			mSwapChain.GetAddressOf()));
	}

	void CreateDescHeaps() {
		//RTV
		{
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
			rtvHeapDesc.NumDescriptors = mSwapChainBufferCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			rtvHeapDesc.NodeMask = 0;
			ThrowIfFailed(md3dDevice->CreateDescriptorHeap(
				&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));
		}

		//DSV
		{
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			dsvHeapDesc.NodeMask = 0;
			ThrowIfFailed(md3dDevice->CreateDescriptorHeap(
				&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));
		}

		//SRV
		{
			D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
			srvHeapDesc.NumDescriptors = 1;
			srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			srvHeapDesc.NodeMask = 0;
			ThrowIfFailed(md3dDevice->CreateDescriptorHeap(
				&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap)));
		}

		//CBV
		{
			D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
			cbvHeapDesc.NumDescriptors = 1000;
			cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			cbvHeapDesc.NodeMask = 0;
			ThrowIfFailed(md3dDevice->CreateDescriptorHeap(
				&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));

			mPassCB = std::make_unique<UploadBuffer<PassConstants>>(md3dDevice.Get(), mPassCBCount, true);
			mMaterialCB = std::make_unique<UploadBuffer<MaterialConstants>>(md3dDevice.Get(), mMaterialCBCount, true);
			mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), 100, true);
		}

	}

	export void InitDX() {
		//Enable debug layer
		ComPtr<ID3D12Debug> debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();

		//Create DXGI Factory
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory)));

		//Create d3d device.
		HRESULT hr = D3D12CreateDevice(
			nullptr,             // default adapter
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&md3dDevice));

		// Fallback to WARP device.
		if (FAILED(hr))
		{
			ComPtr<IDXGIAdapter> pWarpAdapter;
			ThrowIfFailed(mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

			ThrowIfFailed(D3D12CreateDevice(
				pWarpAdapter.Get(),
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&md3dDevice)));
		}

		//Create Fence
		ThrowIfFailed(md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&mFence)));

		//Fill Descriptor Size
		mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		mCbvSrvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// Check 4X MSAA quality support for our back buffer format.
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
		msQualityLevels.Format = mBackBufferFormat;
		msQualityLevels.SampleCount = 4;
		msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQualityLevels.NumQualityLevels = 0;
		ThrowIfFailed(md3dDevice->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&msQualityLevels,
			sizeof(msQualityLevels)));

		m4xMsaaQuality = msQualityLevels.NumQualityLevels;
		assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

		CreateCommandObjects();
		CreateSwapChain();
		CreateDescHeaps();
	}

	export void FlushCommandQueue()
	{
		// Advance the fence value to mark commands up to this fence point.
		mCurrentFence++;

		// Add an instruction to the command queue to set a new fence point.  Because we 
		// are on the GPU timeline, the new fence point won't be set until the GPU finishes
		// processing all the commands prior to this Signal().
		ThrowIfFailed(mCommandQueue->Signal(mFence.Get(), mCurrentFence));

		// Wait until the GPU has completed commands up to this fence point.
		if (mFence->GetCompletedValue() < mCurrentFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

			// Fire event when GPU hits current fence.  
			ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFence, eventHandle));

			// Wait until the GPU hits current fence event is fired.
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}

	export ID3D12Resource* CurrentBackBuffer() {
		return mSwapChainBuffer[mCurrentBackBufferCount].Get();
	}
	export D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
			mCurrentBackBufferCount,
			mRtvDescriptorSize
		);
	}
	export D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() {
		return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	export void OnResize() {
		assert(md3dDevice);
		assert(mSwapChain);
		assert(mCommandAllocator);

		// Flush before changing any resources.
		FlushCommandQueue();

		ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), nullptr));

		// Release the previous resources we will be recreating.
		for (int i = 0; i < mSwapChainBufferCount; ++i) {
			mSwapChainBuffer[i].Reset();
		}

		mDepthStencilBuffer.Reset();

		// Resize the swap chain.
		ThrowIfFailed(mSwapChain->ResizeBuffers(
			mSwapChainBufferCount,
			mClientWidth, mClientHeight,
			mBackBufferFormat,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		mCurrentBackBufferCount = 0;

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < mSwapChainBufferCount; i++)
		{
			ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i])));
			md3dDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
			rtvHeapHandle.Offset(1, mRtvDescriptorSize);
		}

		// Create the depth/stencil buffer and view.
		D3D12_RESOURCE_DESC depthStencilDesc;
		depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthStencilDesc.Alignment = 0;
		depthStencilDesc.Width = mClientWidth;
		depthStencilDesc.Height = mClientHeight;
		depthStencilDesc.DepthOrArraySize = 1;
		depthStencilDesc.MipLevels = 1;

		// Correction 11/12/2016: SSAO chapter requires an SRV to the depth buffer to read from 
		// the depth buffer.  Therefore, because we need to create two views to the same resource:
		//   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
		//   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
		// we need to create the depth buffer resource with a typeless format.  
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
		depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE optClear;
		optClear.Format = mDepthStencilFormat;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;
		ThrowIfFailed(md3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&optClear,
			IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())));

		// Create descriptor to mip level 0 of entire resource using the format of the resource.
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = mDepthStencilFormat;
		dsvDesc.Texture2D.MipSlice = 0;
		md3dDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, DepthStencilView());

		// Transition the resource from its initial state to be used as a depth buffer.
		mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

		// Execute the resize commands.
		ThrowIfFailed(mCommandList->Close());
		ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// Wait until resize is complete.
		FlushCommandQueue();

		// Update the viewport transform to cover the client area.
		mScreenViewport.TopLeftX = 0;
		mScreenViewport.TopLeftY = 0;
		mScreenViewport.Width = static_cast<float>(mClientWidth);
		mScreenViewport.Height = static_cast<float>(mClientHeight);
		mScreenViewport.MinDepth = 0.0f;
		mScreenViewport.MaxDepth = 1.0f;

		mScissorRect = { 0, 0, mClientWidth, mClientHeight };

	}
}

