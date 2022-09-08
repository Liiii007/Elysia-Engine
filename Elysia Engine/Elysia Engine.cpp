#include <stdafx.h>
#include <Tools/Common/d3dUtil.h>

import InputSystem;
import ReflectSystem;
import Mesh;
import GriseoRenderer;
import WorldManager;
import ResourceManager;
import DXDeviceResource;
import Log;
import Profiler;
import Timer;
import CameraSystem;
import Editor.UI;

// Entrypoint
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,_In_ int nCmdShow) {

    // Enable run-time memory check for debug builds.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    try
    {  
        Device::InitDX();
        ReflectSystem::Init();
        InputSystem::Init();
        ResourceManager::Init();
        WorldManager::Init("D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Level\\Level1.json");
        Editor::UI::Init();
        GriseoRenderer::Init(hInstance);

        bool done = false;
        //Main Tick
        while (true) {
            MSG msg;
            while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                if (msg.message == WM_QUIT)
                    done = true;
            }
            if (done)
                break;

            Profiler::NewTick();
            Timer::Tick();

            //SystemBase::SystemTick();
            CameraSystem::Tick();

            //Renderer Tick;
            GriseoRenderer::RenderTick();

            {
                std::string Test{ "Test" };
                Profiler::Auto _(Test);

                for (int i = 0; i < 100; i++) {
                    Profiler::Auto a("Test1");
                }
            }

        }

        //Shutdown
    }
    catch (DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }

    return 0;
}
