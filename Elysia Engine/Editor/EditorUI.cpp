#include <stdafx.h>
#include "EditorUI.h"

import DXDeviceResource;
import ECS;
import Translation;
import GriseoRenderer;
import Log;
using namespace Component;

void EditorUI::Init() {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(DX::mhMainWnd);
	ImGui_ImplDX12_Init(DX::md3dDevice.Get(), DX::mSwapChainBufferCount,
		DXGI_FORMAT_R8G8B8A8_UNORM, DX::mCbvHeap.Get(),
		DX::mSrvHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
		DX::mSrvHeap.Get()->GetGPUDescriptorHandleForHeapStart());

	isInit = true;
}

void EditorUI::Draw() {
	//UnInti->don't draw
	if (!isInit) return;

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


		auto light = Entity::GetEntity("eLight");

		ImGui::SliderFloat("float", &light->GetComponent<Translation>()->position.x, -10, 10);
		

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	static std::shared_ptr<Entity> selectedEntity = nullptr;
	{//Show Entities List
		ImGui::Begin("Entities");

		for (auto iter = Entity::begin(); iter != Entity::end(); iter++) {
			if (ImGui::Selectable(iter->second->name.c_str(), selectedEntity == iter->second))
				selectedEntity = iter->second;
		}

		ImGui::End();
	}

	{//Show Log Data
		ImGui::Begin("Log Output");
		if (ImGui::Button("Clear Logs")) {
			Log::clearLogs();
		}


		auto loggerInfo = Log::GetLogs();
		for (auto& log : *Log::GetLogs()) {

			ImGui::Text(log.c_str());
		}

		ImGui::End();
	}

	{//Show Component Data
		ImGui::Begin("Component");

		if (selectedEntity != nullptr) {
			for (auto& ui : selectedEntity->components) {
				auto base = ui.second->getBase();
				base.lock()->DrawEditorUI();
			}
		}
		
		ImGui::End();
	}

	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DX::mCommandList.Get());
}

EditorUI::~EditorUI() {
	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}