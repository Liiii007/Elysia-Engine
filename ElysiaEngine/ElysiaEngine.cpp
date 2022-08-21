#include "framework.h"
#include "ElysiaEngine.h"
#include "../Renderer/XIIRenderer.h"
#include "../World/WorldManager.h"
#include "../Renderer/Shader.h"

#include "../World/Entity.h"
#include "../Tools/Logger.h"
#include "../Components/Light.h"
#include "../System/LightMoveSystem.h"

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
        Shader initShader = Shader(L"C:\\Users\\LiYU\\source\\repos\\LiquidEngine\\LiquidEngine\\Renderer\\Shaders\\normal.hlsl", "initShader");
        //Shader normalShader = Shader(L"C:\\Users\\LiYU\\source\\repos\\LiquidEngine\\LiquidEngine\\Renderer\\Shaders\\normal.hlsl", "normalShader");
        
        //System Test
        LightMoveSystem system1;

        Entity e2("e2");
        e2.AppendComponent<Mesh>()
            .AppendComponent<Material>()
            .SetLocation(0, 0, 0)
            .SetRotation(-90, 90, 0);
          
        e2.GetComponent<Mesh>()->Init("C:\\Users\\LiYU\\source\\repos\\LiquidEngine\\LiquidEngine\\Resources\\Model\\dawei.fbx");
        e2.GetComponent<Material>()->SetShader(&initShader);

        Entity eLight("eLight");
        eLight
            .SetLocation(3, 3, 3)
            .AppendComponent<Light>()
            .GetComponent<Light>()
                ->SetTarget(XMFLOAT3(0,0,0))
                ->ReturnParentEntity()
        ;

        Singleton<XIIRenderer>::Get()->Init(hInstance);

        auto a = Log::GetLogs();

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
