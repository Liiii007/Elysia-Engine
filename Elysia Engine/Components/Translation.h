#pragma once
#include "../Interface/ISerializable.h"
#include <DirectXColors.h>
#include <vector>
using namespace DirectX;

class Translation
{
public:
	Translation(){};
	Translation(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale):position(position), rotation(rotation), scale(scale){};

	XMFLOAT3 position{ 0,0,0 };
	XMFLOAT3 rotation{ 0,0,0 };
	XMFLOAT3 scale{ 1,1,1 };
};

