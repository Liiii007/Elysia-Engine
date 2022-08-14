#include "framework.h"
#include "LiquidEngine.h"
#include "../Renderer/XIIRenderer.h"
#include "../World/WorldManager.h"

#include "../World/Entity.h"
#include "../Tools/Logger.h"

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
        //if (!Singleton<WorldManager>::Get()->Init()) return 0;
        Singleton<WorldManager>::Get()->Init();
        Entity e1;
        Entity e2;
        e1.translation.rotation = XMVECTOR{ 45, 0, 0 };
        Shader s1 = Shader(L"Renderer\\Shaders\\color.hlsl", "shader1");
        //e2.translation.rotation = XMVECTOR{ 45, 0, 0 };
        Singleton<XIIRenderer>::Get()->Init(hInstance);

        Log::Error("This is an error");
        Log::Warning("This is a warning");

        auto a = Log::getLogs();

        //Tick
        while (true) {
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
