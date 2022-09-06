#include <stdafx.h>
#include <Tools/Common/d3dUtil.h>
#include <Tools/Common/UploadBuffer.h>
#include <Editor/EditorUI.h>

import Mesh;
import Camera;
import InputSystem;
import DXDeviceResource;
import Material;
import Shader;
import ECS;
import Light;
import Definition;
import MaterialData;
import Log;

export module GriseoRenderer;
namespace GriseoRenderer {
	using namespace DX;
	using namespace Component;
	using namespace Resource;
	using namespace DirectX;

	//Init
	export bool Init(HINSTANCE);
	
	bool InitWindow();
	void CreatePassConstantBuffer();
	void CreateObjectConstantBuffer(int objectIndex);
	void CreateMaterialConstantBuffer(int materialIndex);
	//Upload
	void UploadObjectCB(std::shared_ptr<Mesh>);
	void UploadPassCB();
	void UploadMaterialCB(std::shared_ptr<MaterialData> material);

	//Tick
	int RenderTick();
	void Update();

	


	//Render Tick
	void ClearForNextFrame();
	void RenderItem(std::shared_ptr<Mesh> mesh);
	void RenderFrame();


	std::wstring mMainWndCaption = L"XII Renderer";

	Camera mCamera;

	//function
	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return InputSystem::MsgProc(hwnd, msg, wParam, lParam);
		//return Singleton<InputSystem>::Get()->MsgProc(hwnd, msg, wParam, lParam);
	}

	bool Init(HINSTANCE hInstance) {
		mHInstance = hInstance;

		if (!InitWindow()) { return false; }
		DX::InitDX();
		Singleton<EditorUI>::Get()->Init();

		// Do the initial resize code.
		OnResize();

		// Reset the command list to prep for initialization commands.
		ThrowIfFailed(DX::mCommandList->Reset(DX::mCommandAllocator.Get(), nullptr));

		CreatePassConstantBuffer();

		for (auto& it = meshs.begin(); it != meshs.end(); it++) {
			(*it)->UploadVertices();
			(*it)->mObjectIndex = mRendererItemCount;
			CreateObjectConstantBuffer((*it)->mObjectIndex);
			mRendererItemCount++;
		}

		for (auto it : Shader::instances) {
			it.second->Build();
		}

		//Create Material Constant Buffer
		int matCBCount = 0;

		for (auto& mat : MaterialData::materialDatas) {
			if (mat.second == nullptr) {
				Log::Error("MaterialData is empty");
				continue;
			}

			mat.second->matCBIndex = matCBCount;
			CreateMaterialConstantBuffer(matCBCount);
			UploadMaterialCB(mat.second);
			matCBCount++;
		}

		// Execute the initialization commands.
		ThrowIfFailed(DX::mCommandList->Close());
		ID3D12CommandList* cmdsLists[] = { DX::mCommandList.Get() };
		DX::mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// Wait until initialization is complete.
		FlushCommandQueue();

		return true;
	}

	bool InitWindow() {
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
		ShowWindow(mhMainWnd, SW_SHOW);
		UpdateWindow(mhMainWnd);
		return true;
	}

	void CreatePassConstantBuffer() {

		UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));

		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mPassCB->Resource()->GetGPUVirtualAddress();

		UINT heapIndex = 0;
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DX::mCbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));

		DX::md3dDevice->CreateConstantBufferView(
			&cbvDesc,
			handle);
	}

	void CreateMaterialConstantBuffer(int materialIndex) {

		UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

		//GPU offset
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mMaterialCB->Resource()->GetGPUVirtualAddress();
		cbAddress += matCBByteSize * materialIndex;

		//CPU offset
		int heapIndex = mPassCBCount + materialIndex;
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DX::mCbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = matCBByteSize;

		DX::md3dDevice->CreateConstantBufferView(
			&cbvDesc,
			handle);
	}

	void CreateObjectConstantBuffer(int objectIndex) {

		UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

		//GPU offset
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
		cbAddress += objCBByteSize * objectIndex;

		//CPU offset
		int heapIndex = mPassCBCount + mMaterialCBCount + objectIndex;
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DX::mCbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = objCBByteSize;

		DX::md3dDevice->CreateConstantBufferView(
			&cbvDesc,
			handle);
	}

	void ClearForNextFrame() {
		ThrowIfFailed(mCommandAllocator->Reset());

		// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
		// Reusing the command list reuses memory.
		ThrowIfFailed(DX::mCommandList->Reset(mCommandAllocator.Get(), nullptr));

		DX::mCommandList->RSSetViewports(1, &mScreenViewport);
		DX::mCommandList->RSSetScissorRects(1, &mScissorRect);

		// Indicate a state transition on the resource usage.
		DX::mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		// Clear the back buffer and depth buffer.
		DX::mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::Black, 0, nullptr);
		DX::mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		// Specify the buffers we are going to render to.
		DX::mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());
	}

	void Update() {

		float speed = 0.1f;
		if (InputSystem::GetKey(Key::W) == true) {
			mCamera.pos.x -= speed;
		}
		if (InputSystem::GetKey('A') == true) {
			mCamera.pos.z -= speed;
		}
		if (InputSystem::GetKey('S') == true) {
			mCamera.pos.x += speed;
		}
		if (InputSystem::GetKey('D') == true) {
			mCamera.pos.z += speed;
		}

	}
	void UploadPassCB() {
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
	void UploadObjectCB(std::shared_ptr<Mesh> mesh) {
		XMMATRIX world = mesh->getWorldMatrix();

		ObjectConstants ocb;
		XMStoreFloat4x4(&ocb.gWorld, XMMatrixTranspose(world));

		mObjectCB->CopyData(mesh->mObjectIndex, ocb);
	}
	void UploadMaterialCB(std::shared_ptr<MaterialData> material) {
		MaterialConstants mcb{ material->materialConstants };
		mMaterialCB->CopyData(material->matCBIndex, mcb);
	}
	void RenderItem(std::shared_ptr<Mesh> mesh) {


		//set object CB
		int objectCbvIndex = mPassCBCount + mMaterialCBCount + mesh->mObjectIndex;
		auto objectCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DX::mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		objectCbvHandle.Offset(objectCbvIndex, mCbvSrvUavDescriptorSize);

		//Set by mesh
		DX::mCommandList->IASetVertexBuffers(0, 2, mesh->VertexBufferView());
		DX::mCommandList->IASetIndexBuffer(mesh->IndexBufferView());
		DX::mCommandList->IASetPrimitiveTopology(mesh->Topology);
		DX::mCommandList->SetGraphicsRootDescriptorTable(0, objectCbvHandle);

		//FIXED:GPU Instancing
		DX::mCommandList->DrawIndexedInstanced(
			mesh->getIndices()->size(),
			1, 0, 0, 0);
	}
	void RenderFrame() {
		DX::mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		ThrowIfFailed(DX::mCommandList->Close());

		ID3D12CommandList* cmdsLists[] = { DX::mCommandList.Get() };
		DX::mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		ThrowIfFailed(mSwapChain->Present(0, 0));
		mCurrentBackBufferCount = (mCurrentBackBufferCount + 1) % mSwapChainBufferCount;

		FlushCommandQueue();
	}

	export int RenderTick() {
		ClearForNextFrame();
		Update();
		UploadPassCB();

		//Render Item
		ID3D12DescriptorHeap* descriptorHeaps[] = { DX::mCbvHeap.Get(), };
		DX::mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

		std::shared_ptr<Shader> lastShader = Shader::instances["initShader"];
		DX::mCommandList->SetGraphicsRootSignature(lastShader->mRootSignature.Get());
		DX::mCommandList->SetPipelineState(lastShader->mPSO.Get());

		//set pass CB
		int passCbvIndex = 0;
		auto passCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DX::mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		passCbvHandle.Offset(passCbvIndex, mCbvSrvUavDescriptorSize);
		DX::mCommandList->SetGraphicsRootDescriptorTable(1, passCbvHandle);


		for (auto iter = Entity::begin(); iter != Entity::end(); iter++) {
			if (iter->second->HasComponent<Mesh>() && iter->second->HasComponent<Material>()) {
				std::shared_ptr<Entity> entity = iter->second;
				//Check Material Component state
				if (!entity->GetComponent<Material>()->enabled) {
					Log::Error("Material unenabled:");
					continue;
				}


				//Set correct Shader, avoiding switching PSO frequently
				std::shared_ptr<Shader> currentShader = entity->GetComponent<Material>()->getShader();
				if (*currentShader != *lastShader) {
					DX::mCommandList->SetGraphicsRootSignature(currentShader->mRootSignature.Get());
					DX::mCommandList->SetPipelineState(currentShader->mPSO.Get());
					lastShader = currentShader;
				}

				//set material CB
				int materialCbvIndex = mPassCBCount + entity->GetComponent<Material>()->GetCBIndex();
				auto materialCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DX::mCbvHeap->GetGPUDescriptorHandleForHeapStart());
				materialCbvHandle.Offset(materialCbvIndex, mCbvSrvUavDescriptorSize);
				DX::mCommandList->SetGraphicsRootDescriptorTable(2, materialCbvHandle);

				UploadObjectCB(entity->GetComponent<Mesh>());

				RenderItem(entity->GetComponent<Mesh>());
			}

		}

		Singleton<EditorUI>::Get()->Draw();

		RenderFrame();
		mFrameCount++;
		return 0;
	}

}
