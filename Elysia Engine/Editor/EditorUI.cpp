#include "EditorUI.h"
#include <Renderer/XIIRenderer.h>
#include <Tools/Singleton.h>
#include <World/Entity.h>
#include <Components/FullComponentHeader.h>
#include <System/FullSystemHeader.h>

//imgui
#include <Renderer/imgui/imgui.h>
#include <Renderer/imgui/imgui_impl_win32.h>
#include <Renderer/imgui/imgui_impl_dx12.h>

void EditorUI::Init() {

	auto renderer = Singleton<XIIRenderer>::Get();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(renderer->mhMainWnd);
	ImGui_ImplDX12_Init(renderer->md3dDevice.Get(), renderer->mSwapChainBufferCount,
		DXGI_FORMAT_R8G8B8A8_UNORM, renderer->mCbvHeap.Get(),
		renderer->mSrvHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
		renderer->mSrvHeap.Get()->GetGPUDescriptorHandleForHeapStart());

	isInit = true;
}

void EditorUI::Draw() {
	//UnInti->don't draw
	if (!isInit) return;

	auto renderer = Singleton<XIIRenderer>::Get();

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	
	{
		static int counter = 0;

		ImGui::Begin("Light Editor");

		ImGui::Checkbox("Demo Window", &show_demo_window);


		auto light = Entity::entities["eLight"];

		ImGui::SliderFloat("float", &light->translation.position.x, -10, 10);
		

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	static Entity* selectedEntity = nullptr;
	{
		ImGui::Begin("Entities");

		for (auto& entity : Entity::entities) {
			if (ImGui::Selectable(entity.second->name.c_str(), selectedEntity == entity.second))
				selectedEntity = entity.second;
		}

		ImGui::End();
	}

	{
		ImGui::Begin("Debug");

		

		ImGui::End();
	}

	{
		ImGui::Begin("Component");

		//Translation
		if (selectedEntity != nullptr) {
			ImGui::DragFloat3("Location", &selectedEntity->translation.position.x, 0.1f);
			ImGui::DragFloat3("Rotation", &selectedEntity->translation.rotation.x, 0.1f);
			ImGui::DragFloat3("Scale", &selectedEntity->translation.scale.x, 0.1f);
			ImGui::Spacing();
		}
		
		ImGui::End();
	}

	ImGui::Render();

	// Render Dear ImGui graphics

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), renderer->mCommandList.Get());

}

EditorUI::~EditorUI() {
	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}