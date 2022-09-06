#pragma once

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <Tools/Common/d3dUtil.h>
#include <Tools/Common/UploadBuffer.h>

//Relative Component
class Shader;
class MaterialData;
struct ObjectConstants;
struct PassConstants;
struct MaterialConstants;

import Mesh;
import Camera;
using namespace Component;

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
