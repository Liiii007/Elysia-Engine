#include "framework.h"
#include "LiquidEngine.h"
#include "../Renderer/XIIRenderer.h"
#include "../World/WorldManager.h"
#include "../Renderer/Shader.h"

#include "../World/Entity.h"
#include "../Tools/Logger.h"
#include "../Components/Light.h"
#include "../Interface/IComponent.h"

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
        Entity e1;
        e1.setName("e1")
          .appendComponent<Mesh>()
          .appendComponent<Material>()
          .loadMesh("C:\\Users\\LiYU\\source\\repos\\LiquidEngine\\LiquidEngine\\Resources\\Model\\dawei.fbx");

        Entity e2;
        e2.setName("e2")
          .appendComponent<Mesh>()
          .appendComponent<Material>()
          .setLocation( -1, 0, 0)
          .setRotation(-90, 0, 0)
          .loadMesh("C:\\Users\\LiYU\\source\\repos\\LiquidEngine\\LiquidEngine\\Resources\\Model\\dawei.fbx");


        Shader s1 = Shader(L"Renderer\\Shaders\\color.hlsl", "shader1");

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
