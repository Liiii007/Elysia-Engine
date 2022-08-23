#include "framework.h"
#include "Renderer/XIIRenderer.h"
#include "World/WorldManager.h"
#include "Resources/ResourceManager.h"

#include "Tools/Logger.h"
#include "System/LightMoveSystem.h"

#include "Tools/JSONHandler.h"
#include "Tools/Reflect.h"


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
            SystemBase::SystemTick();
            Singleton<XIIRenderer>::Get()->RenderTick();
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
