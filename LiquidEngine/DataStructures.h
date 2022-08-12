#pragma once
#include <DirectXColors.h>
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

using UINT = unsigned int;

struct Vertex1 {
	DirectX::XMFLOAT3 Pos;//offset:0
	DirectX::XMFLOAT3 Normal;//offset:12
	DirectX::XMFLOAT2 Texcoord;//offset:24
};

struct ObjectConstants {
	DirectX::XMFLOAT4X4 MVP;
};

struct Vertex {
	DirectX::XMFLOAT3 Pos;
};

struct Vertex_C {
	DirectX::XMFLOAT4 Color;
};



