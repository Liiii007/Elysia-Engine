#include <stdafx.h>
export module Definition;

template<typename T>
using Action = std::function<void(T)>;

export struct ObjectConstants {
	DirectX::XMFLOAT4X4 gWorld;
	DirectX::XMFLOAT4 gColor;
};

export struct PassConstants {
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

export struct MaterialConstants {
	//4byte
	DirectX::XMFLOAT4 diffuseAlbedo;

	//4byte
	DirectX::XMFLOAT3 fresnelR0;
	float roughness;
};