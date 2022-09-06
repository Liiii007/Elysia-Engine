#pragma once
//STD
#include <vector>
#include <string>
#include <any>
#include <unordered_map>
#include <chrono>
#include <filesystem>
#include <memory>
#include <functional>
#include <algorithm>

//Tools
#include <Tools/Logger.h>
#include <Tools/Action.h>
#include <Tools/JSONHandler.h>

//Windows platform
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <wrl.h>
#include <DirectXColors.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

//3rd party
#include <Renderer/imgui/imgui.h>
#include <Renderer/imgui/imgui_impl_win32.h>
#include <Renderer/imgui/imgui_impl_dx12.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include <rapidjson/document.h>

#include <Renderer/ShaderConstantBufferStruct.h>

