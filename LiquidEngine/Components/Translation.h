#pragma once
#include "../Interface/ISerializable.h"
#include <DirectXColors.h>
using namespace DirectX;
class Translation
{
public:
	XMVECTOR position{ 0,0,0 };
	XMVECTOR rotation{ 0,0,0 };
	XMVECTOR scale{ 1,1,1 };

	XMMATRIX getWorldMatrix() {

	}
};

