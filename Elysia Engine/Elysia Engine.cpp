#include <stdafx.h>
#include "Resources/ResourceManager.h"
#include <Editor/EditorUI.h>
#include <Tools/Common/d3dUtil.h>


import InputSystem;
import ReflectSystem;
import Mesh;
import GriseoRenderer;
import WorldManager;
import Log;

// Entrypoint
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,_In_ int nCmdShow) {

    // Enable run-time memory check for debug builds.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    try
    {  
        ReflectSystem::Init();
        InputSystem::Init();
        Singleton<ResourceManager>::Get()->Init();
        Singleton<WorldManager>::Get()->Init("D:\\Working\\VS Projects\\Elysia Engine\\Elysia Engine\\Resources\\Level\\Level1.json");
        GriseoRenderer::Init(hInstance);

#ifdef EDITOR_ENABLE
        Singleton<EditorUI>::Get()->Init();
#endif // EDITOR_ENABLE


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
            //SystemBase::SystemTick();
            GriseoRenderer::RenderTick();
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
