#include "GriseoRenderer.h"
#include <World/Entity.h>
#include <Renderer/Shader.h>
#include <Components/FullComponentHeader.h>
#include <System/FullSystemHeader.h>

#include <Editor/EditorUI.h>


LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Singleton<InputSystem>::Get()->MsgProc(hwnd, msg, wParam, lParam);
}

bool GriseoRenderer::Init(HINSTANCE hInstance) {
	mHInstance = hInstance;

	if (!InitWindow())   { return false; }
	if (!InitDirect3D()) { return false; }

	// Do the initial resize code.
	OnResize();

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), nullptr));

	CreatePassConstantBuffer();

	for (auto& it = MeshRenderer::getMeshList()->begin(); it != MeshRenderer::getMeshList()->end(); it++) {
		(*it)->UploadVertices();
		(*it)->mObjectIndex = mRendererItemCount;
		CreateObjectConstantBuffer((*it)->mObjectIndex);
		mRendererItemCount++;
	}

	for (auto it : Shader::shaders) {
		it.second->Build();
	}

	for (auto it : Material::materials) {
		CreateMaterialConstantBuffer(it.second->matCBIndex.value_or<int>(0));	
	}

	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();

	
	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);
	return true;
}

bool GriseoRenderer::InitWindow() {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	//Window Size
	RECT R = { 0, 0, mClientWidth, mClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	mhMainWnd = CreateWindow(L"MainWnd", mMainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mHInstance, 0);
	if (!mhMainWnd)
	{
		Log::Error("CreateWindow Failed.");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	return true;
}

bool GriseoRenderer::InitDirect3D() {

	ComPtr<ID3D12Debug> debugController;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();

	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory)));

	// Try to create hardware device.
	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,             // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&md3dDevice));

	// Fallback to WARP device.
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
		ThrowIfFailed(mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&md3dDevice)));
	}

	ThrowIfFailed(md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&mFence)));

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

	return true;
}

void GriseoRenderer::CreateCommandObjects() {
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

	ThrowIfFailed(md3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(mCommandAllocator.GetAddressOf())));

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

void GriseoRenderer::CreateSwapChain() {
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

void GriseoRenderer::CreateDescHeaps() {
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
		cbvHeapDesc.NumDescriptors = 100;
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

void GriseoRenderer::CreatePassConstantBuffer() {
	
	UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));

	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mPassCB->Resource()->GetGPUVirtualAddress();

	UINT heapIndex = 0;
	auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvHeap->GetCPUDescriptorHandleForHeapStart());
	handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));

	md3dDevice->CreateConstantBufferView(
		&cbvDesc,
		handle);
}

void GriseoRenderer::CreateMaterialConstantBuffer(int materialIndex) {

	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	//GPU offset
	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mMaterialCB->Resource()->GetGPUVirtualAddress();
	cbAddress += matCBByteSize * materialIndex;

	//CPU offset
	int heapIndex = mPassCBCount + materialIndex;
	auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvHeap->GetCPUDescriptorHandleForHeapStart());
	handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = matCBByteSize;

	md3dDevice->CreateConstantBufferView(
		&cbvDesc,
		handle);
}

void GriseoRenderer::CreateObjectConstantBuffer(int objectIndex) {
	
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	//GPU offset
	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
	cbAddress += objCBByteSize * objectIndex;

	//CPU offset
	int heapIndex = mPassCBCount + mMaterialCBCount + objectIndex;
	auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvHeap->GetCPUDescriptorHandleForHeapStart());
	handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = objCBByteSize;

	md3dDevice->CreateConstantBufferView(
		&cbvDesc,
		handle);
}



void GriseoRenderer::OnResize() {
	assert(md3dDevice);
	assert(mSwapChain);
	assert(mCommandAllocator);

	// Flush before changing any resources.
	FlushCommandQueue();

	ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), nullptr));

	// Release the previous resources we will be recreating.
	for (int i = 0; i < mSwapChainBufferCount; ++i)
		mSwapChainBuffer[i].Reset();
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

void GriseoRenderer::FlushCommandQueue()
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

ID3D12Resource* GriseoRenderer::CurrentBackBuffer()const {
	return mSwapChainBuffer[mCurrentBackBufferCount].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE GriseoRenderer::CurrentBackBufferView()const {
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrentBackBufferCount,
		mRtvDescriptorSize
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE GriseoRenderer::DepthStencilView()const {
	return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

void GriseoRenderer::ClearForNextFrame() {
	ThrowIfFailed(mCommandAllocator->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), nullptr));

	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::Black, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());
}


void GriseoRenderer::Update() {
	
}

void GriseoRenderer::UploadPassCB() {
	XMMATRIX view = mCamera.getViewMatrix();
	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.3f * MathHelper::Pi, (FLOAT)mClientWidth / (FLOAT)mClientHeight, 1.0f, 1000.0f);
	XMMATRIX viewProj = view * proj;
	auto light = Entity::GetEntity("eLight")->GetComponent<Light>();

	PassConstants pcb{};

	XMStoreFloat4x4(&pcb.gViewProj, XMMatrixTranspose(viewProj));
	pcb.viewPos = mCamera.GetPosition();
	pcb.viewDir = mCamera.GetDirection();
	pcb.lightPos = light->GetPosition();
	pcb.lightDir = light->GetDirection();
	pcb.lightColor = light->GetColor();
	pcb.lightPower = light->GetPower();

	mPassCB->CopyData(0, pcb);
}

void GriseoRenderer::UploadObjectCB(Mesh* mesh) {
	XMMATRIX world = mesh->getWorldMatrix();

	ObjectConstants ocb;
	XMStoreFloat4x4(&ocb.gWorld, XMMatrixTranspose(world));
	if (mesh->mObjectIndex == 0) {
		ocb.gColor = XMFLOAT4(1, 1, 0, 1);
	}
	else {
		ocb.gColor = XMFLOAT4(0, 1, 0, 1);
	}
	mObjectCB->CopyData(mesh->mObjectIndex, ocb);
}

void GriseoRenderer::RenderItem(Mesh* mesh) {

	//set object CB
	int objectCbvIndex = mPassCBCount + mMaterialCBCount + mesh->mObjectIndex;
	auto objectCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	objectCbvHandle.Offset(objectCbvIndex, mCbvSrvUavDescriptorSize);
	
	//Set by mesh
	mCommandList->IASetVertexBuffers(0, 2, mesh->VertexBufferView());
	mCommandList->IASetIndexBuffer(mesh->IndexBufferView());
	mCommandList->IASetPrimitiveTopology(mesh->Topology);
	mCommandList->SetGraphicsRootDescriptorTable(0, objectCbvHandle);

	//FIXED:GPU Instancing
	mCommandList->DrawIndexedInstanced(
		mesh->getIndices()->size(),
		1, 0, 0, 0);
}

void GriseoRenderer::RenderFrame() {
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	ThrowIfFailed(mCommandList->Close());

	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrentBackBufferCount = (mCurrentBackBufferCount + 1) % mSwapChainBufferCount;

	FlushCommandQueue();
}

int GriseoRenderer::RenderTick() {
	ClearForNextFrame();
	Update();
	UploadPassCB();

	//Render Item
	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get(), };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	Shader* lastShader = Shader::shaders["initShader"];
	mCommandList->SetGraphicsRootSignature(lastShader->mRootSignature.Get());
	mCommandList->SetPipelineState(lastShader->mPSO.Get());

	//set pass CB
	int passCbvIndex = 0;
	auto passCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	passCbvHandle.Offset(passCbvIndex, mCbvSrvUavDescriptorSize);
	mCommandList->SetGraphicsRootDescriptorTable(1, passCbvHandle);

	for (auto it = MeshRenderer::getMeshList()->begin(); it != MeshRenderer::getMeshList()->end(); it++) {
		Shader* s = (*it)->parentEntity->GetComponent<Material>()->getShader();
		if (*s != *lastShader) {
			mCommandList->SetGraphicsRootSignature(s->mRootSignature.Get());
			mCommandList->SetPipelineState(s->mPSO.Get());
			lastShader = s;
		}
		
		UploadObjectCB(*it);
		RenderItem(*it);
	}

	Singleton<EditorUI>::Get()->Draw();
	
	RenderFrame();
	mFrameCount++;
	return 0;
}

LRESULT GriseoRenderer::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			//mAppPaused = true;
			//mTimer.Stop();
		}
		else
		{
			//mAppPaused = false;
			//mTimer.Start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		mClientWidth = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);
		if (md3dDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				//mAppPaused = true;
				//mMinimized = true;
				//mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				//mAppPaused = false;
				//mMinimized = false;
				//mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{


			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:

		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:

		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_KEYUP:
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void GriseoRenderer::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void GriseoRenderer::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void GriseoRenderer::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		//mTheta += dx;
		//mPhi += dy;

		// Restrict the angle mPhi.
		//mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		//mRadius += dx - dy;

		// Restrict the radius.
		//mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}
