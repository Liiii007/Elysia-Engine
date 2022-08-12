#include "framework.h"
#include "LiquidEngine.h"
#include "../Renderer/XIIRenderer.h"
#include "../World/WorldManager.h"

#include"../ABox.h"

// Entrypoint
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        //Initialization
        //if (!Singleton<XIIRenderer>::Get()->Init(hInstance)) return 0;
        //if (!Singleton<WorldManager>::Get()->Init()) return 0;
        ABox testBox;
        Singleton<WorldManager>::Get()->Init();
        Singleton<XIIRenderer>::Get()->Init(hInstance);

        //Tick
        while (true) {
            //Singleton<XIIRenderer>::Get()->RenderTick();
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
