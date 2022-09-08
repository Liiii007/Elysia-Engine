#include <stdafx.h>

#include <3rd/imgui/imgui_impl_dx12.h>
#include <3rd/imgui/imgui_impl_win32.h>



import DXDeviceResource;
import ECS;
import Log;
import Profiler;
import Timer;

export module Editor.UI;

namespace Editor {
	namespace UI {

		export std::vector<std::function<void(void)>> systemUI;
		
		bool isInit{ false };
		bool show_demo_window = false;
		bool show_profiler = false;
		bool show_another_window = false;

		export bool IsInit() {
			return isInit;
		}

		export void Init() {
			if (isInit) {
				return;
			}

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsLight();

			// Setup Platform/Renderer backends
			ImGui_ImplWin32_Init(Device::mhMainWnd);
			ImGui_ImplDX12_Init(Device::md3dDevice.Get(), Device::mSwapChainBufferCount,
				DXGI_FORMAT_R8G8B8A8_UNORM, Device::mCbvHeap.Get(),
				Device::mSrvHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
				Device::mSrvHeap.Get()->GetGPUDescriptorHandleForHeapStart());

			isInit = true;
		}

		export void Draw() {
			//UnInti->don't draw
			if (!isInit) return;
			
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			// Start the Dear ImGui frame
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

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
				if (ImGui::Button("Profiler")) {
					show_profiler = !show_profiler;
					Log::Info("Profiler switch");
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

			{
				for (auto& sys : systemUI) {
					sys();
				}
			}

			{
				if (show_profiler) {
					ImGui::Begin("Profiler");

					std::string singleTick;
					for (auto& tk : Profiler::tickTime) {
						singleTick = tk.first + " " + FloatToString(tk.second*1000);
						ImGui::Text(singleTick.c_str());
					}

					ImGui::End();
				}
			}

			/* {
				Profiler::Auto a("Unit Test");
				Profiler::RecordStart("Test");

				for (int i = 0; i < 100000; i++) {
					Profiler::Auto _("abcd");
				}

				auto m = Profiler::RecordStop("Test");

				ImGui::Begin("Profiler Unit Test");
				ImGui::Text(FloatToString(m * 1000).c_str());
				ImGui::End();
			}*/

			ImGui::Render();

			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Device::mCommandList.Get());
		}

		export void Shutdown() {
			// Cleanup
			ImGui_ImplDX12_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}
	}
}
