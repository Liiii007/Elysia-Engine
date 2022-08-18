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
        //Initialization
        //if (!Singleton<WorldManager>::Get()->Init()) return 0;
        //Debuging

        

        //Singleton<WorldManager>::Get()->Init();
        Entity e1{"e1"};
        e1.appendComponent<Mesh>();
        e1.appendComponent<Material>();
        e1.loadMesh("C:\\Users\\LiYU\\source\\repos\\LiquidEngine\\LiquidEngine\\Resources\\Model\\dawei.fbx");
        
        auto 
        //Entity e2{ "e2" };
        //e2.loadMesh("C:\\Users\\LiYU\\source\\repos\\LiquidEngine\\LiquidEngine\\Resources\\Model\\dawei.fbx");

        //Entity e3{ "e3" };
        //e3.loadMesh("C:\\Users\\LiYU\\source\\repos\\LiquidEngine\\LiquidEngine\\Resources\\Model\\dawei.fbx");
        //e3.setLocation(-1, 0, 0);

        //Entity e4{ "Component Test" };
        //e4.appendComponent<Material>();
        //auto c = e4.getComponent<Material>();

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
