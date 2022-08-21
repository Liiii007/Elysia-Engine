#include "framework.h"
#include "Renderer/XIIRenderer.h"
#include "World/WorldManager.h"
#include "Resources/ResourceManager.h"

#include "World/Entity.h"
#include "Tools/Logger.h"
#include "Components/Light.h"
#include "System/LightMoveSystem.h"
#include <assimp/ai_assert.h>


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
        Singleton<ResourceManager>::Get()->Init();
        Singleton<WorldManager>::Get()->Init();
        Singleton<XIIRenderer>::Get()->Init(hInstance);
        

        //System Test
        LightMoveSystem system1;

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
