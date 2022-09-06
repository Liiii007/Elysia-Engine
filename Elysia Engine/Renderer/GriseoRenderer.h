#pragma once

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <Tools/Common/d3dUtil.h>
#include <Tools/Common/UploadBuffer.h>
#include "../Renderer/SphereCamera.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

import DXDeviceResource;

using namespace DX;

//Relative Component
class Mesh;
class Shader;
class Material;
class MaterialData;
struct ObjectConstants;
struct PassConstants;
struct MaterialConstants;

class GriseoRenderer
{
public:

	bool Init(HINSTANCE);
	//Upload
	void UploadObjectCB(std::shared_ptr<Mesh>);
	void UploadPassCB();
	void UploadMaterialCB(std::shared_ptr<MaterialData> material);

	//Tick
	int RenderTick();
	void Update();


public:
	//Init
	bool InitWindow();

	void CreatePassConstantBuffer();
	void CreateObjectConstantBuffer(int objectIndex);
	void CreateMaterialConstantBuffer(int materialIndex);

	
	//Render Tick
	void ClearForNextFrame();
	void RenderItem(std::shared_ptr<Mesh> mesh);
	void RenderFrame();

	
	std::wstring mMainWndCaption = L"XII Renderer";

	SphereCamera mCamera;
	
};
