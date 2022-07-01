#pragma once
#include "ISerializable.h"

class Translation
{
public:
	float location[3]{ 0,0,0 };
	float rotation[3]{ 0,0,0 };
	float scale[3]{ 1,1,1 };
};

