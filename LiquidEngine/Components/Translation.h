#pragma once
#include "../Interface/ISerializable.h"
#include <DirectXColors.h>
#include <vector>
using namespace DirectX;

class Translation
{
public:
	Translation(){};
	Translation(XMVECTOR position, XMVECTOR rotation, XMVECTOR scale):position(position), rotation(rotation), scale(scale){};

	XMVECTOR position{ 0,0,0 };
	XMVECTOR rotation{ 0,0,0 };
	XMVECTOR scale{ 1,1,1 };
};

