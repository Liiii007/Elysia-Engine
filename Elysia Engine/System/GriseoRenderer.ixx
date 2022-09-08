#include <stdafx.h>
#include <Tools/Common/d3dUtil.h>
#include <Tools/Common/UploadBuffer.h>

import Mesh;
import Camera;
import InputSystem;
import DXDeviceResource;
import Material;
import Shader;
import ECS;
import DirectLight;
import Definition;
import MaterialData;
import Log;
import Editor.UI;
import Translation;
import Profiler;
import CameraSystem;

export module GriseoRenderer;
namespace GriseoRenderer {
	using namespace Device;
	using namespace Component;
	using namespace Resource;
	using namespace DirectX;

	//Init
	export bool Init(HINSTANCE);
	std::string ProfilerEvent{ "Render Tick" };
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
	void ClearForNextFrame();
	void RenderItem(std::shared_ptr<Mesh> mesh);
	void RenderFrame();
	void DrawEditorUI();

	

	//function
	

	bool Init(HINSTANCE hInstance) {
		mHInstance = hInstance;

		Editor::UI::systemUI.emplace_back(DrawEditorUI);

		// Do the initial resize code.
		OnResize();

		// Reset the command list to prep for initialization commands.
		ThrowIfFailed(Device::mCommandList->Reset(Device::mCommandAllocator.Get(), nullptr));

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
		ThrowIfFailed(Device::mCommandList->Close());
		ID3D12CommandList* cmdsLists[] = { Device::mCommandList.Get() };
		Device::mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// Wait until initialization is complete.
		FlushCommandQueue();

		return true;
	}

	

	void CreatePassConstantBuffer() {

		UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));

		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mPassCB->Resource()->GetGPUVirtualAddress();

		UINT heapIndex = 0;
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(Device::mCbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));

		Device::md3dDevice->CreateConstantBufferView(
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
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(Device::mCbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = matCBByteSize;

		Device::md3dDevice->CreateConstantBufferView(
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
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(Device::mCbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(heapIndex, mCbvSrvUavDescriptorSize);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = objCBByteSize;

		Device::md3dDevice->CreateConstantBufferView(
			&cbvDesc,
			handle);
	}

	void ClearForNextFrame() {
		ThrowIfFailed(mCommandAllocator->Reset());

		// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
		// Reusing the command list reuses memory.
		ThrowIfFailed(Device::mCommandList->Reset(mCommandAllocator.Get(), nullptr));

		Device::mCommandList->RSSetViewports(1, &mScreenViewport);
		Device::mCommandList->RSSetScissorRects(1, &mScissorRect);

		// Indicate a state transition on the resource usage.
		Device::mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		// Clear the back buffer and depth buffer.
		Device::mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::Black, 0, nullptr);
		Device::mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		// Specify the buffers we are going to render to.
		Device::mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());
	}

	void Update() {
		auto mCamera = Entity::GetEntity("cam")->GetComponent<Translation>();

		float speed = 0.1f;
		if (InputSystem::GetKey(Key::W) == true) {
			mCamera->position.x -= speed;
		}
		if (InputSystem::GetKey('A') == true) {
			mCamera->position.z -= speed;
		}
		if (InputSystem::GetKey('S') == true) {
			mCamera->position.x += speed;
		}
		if (InputSystem::GetKey('D') == true) {
			mCamera->position.z += speed;
		}

	}
	void UploadPassCB() {
		auto mCamera = CameraSystem::activeCamera.lock();
		XMMATRIX view = mCamera->getViewMatrix();
		XMMATRIX proj = XMMatrixPerspectiveFovLH(0.3f * MathHelper::Pi, (FLOAT)mClientWidth / (FLOAT)mClientHeight, 1.0f, 1000.0f);
		XMMATRIX viewProj = view * proj;
		auto light = Entity::GetEntity("eLight")->GetComponent<DirectLight>();

		PassConstants pcb{};

		XMStoreFloat4x4(&pcb.gViewProj, XMMatrixTranspose(viewProj));
		pcb.viewPos = mCamera->GetPosition();
		pcb.viewDir = mCamera->GetDirection();
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
		auto objectCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(Device::mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		objectCbvHandle.Offset(objectCbvIndex, mCbvSrvUavDescriptorSize);

		//Set by mesh
		Device::mCommandList->IASetVertexBuffers(0, 2, mesh->VertexBufferView());
		Device::mCommandList->IASetIndexBuffer(mesh->IndexBufferView());
		Device::mCommandList->IASetPrimitiveTopology(mesh->Topology);
		Device::mCommandList->SetGraphicsRootDescriptorTable(0, objectCbvHandle);

		//FIXED:GPU Instancing
		Device::mCommandList->DrawIndexedInstanced(
			mesh->getIndices()->size(),
			1, 0, 0, 0);
	}
	void RenderFrame() {
		Device::mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		ThrowIfFailed(Device::mCommandList->Close());

		ID3D12CommandList* cmdsLists[] = { Device::mCommandList.Get() };
		Device::mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		ThrowIfFailed(mSwapChain->Present(0, 0));
		mCurrentBackBufferCount = (mCurrentBackBufferCount + 1) % mSwapChainBufferCount;

		
	}

	export int RenderTick() {
	{

		Profiler::Auto _(ProfilerEvent);

		//Tick1



		//Tick2
		ClearForNextFrame();
		Update();
		UploadPassCB();

		//Render Item
		ID3D12DescriptorHeap* descriptorHeaps[] = { Device::mCbvHeap.Get(), };
		Device::mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

		std::shared_ptr<Shader> lastShader = Shader::instances["initShader"];
		Device::mCommandList->SetGraphicsRootSignature(lastShader->mRootSignature.Get());
		Device::mCommandList->SetPipelineState(lastShader->mPSO.Get());

		//set pass CB
		int passCbvIndex = 0;
		auto passCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(Device::mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		passCbvHandle.Offset(passCbvIndex, mCbvSrvUavDescriptorSize);
		Device::mCommandList->SetGraphicsRootDescriptorTable(1, passCbvHandle);


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
					Device::mCommandList->SetGraphicsRootSignature(currentShader->mRootSignature.Get());
					Device::mCommandList->SetPipelineState(currentShader->mPSO.Get());
					lastShader = currentShader;
				}

				//set material CB
				int materialCbvIndex = mPassCBCount + entity->GetComponent<Material>()->GetCBIndex();
				auto materialCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(Device::mCbvHeap->GetGPUDescriptorHandleForHeapStart());
				materialCbvHandle.Offset(materialCbvIndex, mCbvSrvUavDescriptorSize);
				Device::mCommandList->SetGraphicsRootDescriptorTable(2, materialCbvHandle);

				UploadObjectCB(entity->GetComponent<Mesh>());

				RenderItem(entity->GetComponent<Mesh>());
			}

		}

		Editor::UI::Draw();

		RenderFrame();
		mFrameCount++;

	}
		FlushCommandQueue();
		return 0;
	}

	export void DrawEditorUI() {
		if (Editor::UI::IsInit()) {
			ImGui::Begin("Renderer Info");
			std::string camera = "ActiveCamera:" + CameraSystem::activeCamera.lock()->parentEntity->name;

			ImGui::Text(camera.c_str());

			ImGui::End();
		}
	}

}
