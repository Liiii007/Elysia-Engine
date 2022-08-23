#include "framework.h"
#include "Renderer/XIIRenderer.h"
#include "World/WorldManager.h"
#include "Resources/ResourceManager.h"

#include "World/Entity.h"
#include "Tools/Logger.h"
#include "Components/Light.h"
#include "System/LightMoveSystem.h"

#include "Tools/JSONHandler.h"
#include "Tools/Reflect.h"

//imgui
#include <Renderer/imgui/imgui.h>
#include <Renderer/imgui/imgui_impl_win32.h>
#include <Renderer/imgui/imgui_impl_dx12.h>

// Entrypoint
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Enable run-time memory check for debug builds.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    try
    {  
        Reflect::Init();

        Singleton<ResourceManager>::Get()->Init();
        Singleton<WorldManager>::Get()->Init();
        Singleton<XIIRenderer>::Get()->Init(hInstance);

        //System Test
        LightMoveSystem system1;

        //imgui
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
        ImGui_ImplDX12_Init(renderer->md3dDevice.Get(), renderer->mCurrentBackBufferCount,
            DXGI_FORMAT_R8G8B8A8_UNORM, renderer->mCbvHeap.Get(),
            renderer->mCbvHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
            renderer->mCbvHeap.Get()->GetGPUDescriptorHandleForHeapStart());

        //Main Tick
        while (true) {
            SystemBase::SystemTick();
            Singleton<XIIRenderer>::Get()->RenderTick();
        }
    }
    catch (DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }

    return 0;
}
