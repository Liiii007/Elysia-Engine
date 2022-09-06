#pragma once
#include <vector>
#include <string>
#include <any>
#include <unordered_map>
#include <chrono>
#include <filesystem>
#include <memory>
#include <functional>
#include <algorithm>

#include <Tools/Logger.h>
#include <Tools/Action.h>
#include <Tools/Reflect.h>
#include <World/Entity.h>
#include <Tools/JSONHandler.h>

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

#include <Renderer/imgui/imgui.h>
#include <Renderer/imgui/imgui_impl_win32.h>
#include <Renderer/imgui/imgui_impl_dx12.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <Components/FullComponentHeader.h>
#include <System/FullSystemHeader.h>
#include <Renderer/ShaderConstantBufferStruct.h>