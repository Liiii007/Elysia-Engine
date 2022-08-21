#pragma once

#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include "Tools/Common/d3dUtil.h"

#include "Interface/IComponent.h"
#include "Interface/ComponentBase.h"

using namespace DirectX;

class Entity;

class Light : public ComponentBase
{

public:
	
	static std::string componentName;
	std::string name;

	Light(Entity* entity);

	//Regist to reflect
	static void Bind();
	//Parse init data from json file
	static void Parse(Entity& entity, const rapidjson::Value& parm);

	Light* SetPosition  (const XMFLOAT3& position);
	Light* SetTarget    (const XMFLOAT3& direction);
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetDirection();

	Entity& ReturnParentEntity();

private:
	XMFLOAT3 mPosition;
	XMFLOAT3 mTarget;
	//XMFLOAT3 mDirection;
};

