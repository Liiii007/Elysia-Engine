#pragma once

#include <DirectXMath.h>

//4float per group

struct ObjectConstants {
	DirectX::XMFLOAT4X4 gWorld;
	DirectX::XMFLOAT4 gColor;
};

struct PassConstants {
	DirectX::XMFLOAT4X4 gViewProj;

	//4Byte
	DirectX::XMFLOAT3 viewPos;
	float pad1;

	//4Byte
	DirectX::XMFLOAT3 viewDir;
	float pad2;

	//4Byte
	DirectX::XMFLOAT3 lightDir;
	float pad3;

	//4Byte
	DirectX::XMFLOAT3 lightPos;
	float pad4;

	DirectX::XMFLOAT3 lightColor;
	float lightPower;

};

struct MaterialConstants {
	//4byte
	DirectX::XMFLOAT4 diffuseAlbedo;

	//4byte
	DirectX::XMFLOAT3 fresnelR0;
	float roughness;
};