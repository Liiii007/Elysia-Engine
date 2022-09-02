#pragma once
#include <string>
#include <optional>
#include "../../Interface/IComponent.h"
#include "../../Interface/ComponentBase.h"

#include <DirectXPackedVector.h>

class Shader;
class Entity;

class Material : public ComponentBase {

public:
	Material(Entity* entity);

	static std::string componentName;
	bool enabled{ false };
	std::string name;
	Shader* shader;
	std::optional<int> matCBIndex;
	std::optional<int> diffuseSrvHeapIndex;

	DirectX::XMFLOAT4 diffuseAlbedo{0.2, 0.6, 0.2, 1};
	DirectX::XMFLOAT3 fresnelR0{0.1, 0.1, 0.1};
	float roughness{1};

	void Init(std::string name, std::string shader);

	//Regist to reflect
	static void Bind();
	//Parse init data from json file
	static void Parse(Entity& entity, const rapidjson::Value& parm);

	virtual void DrawEditorUI();

	Shader* getShader();
	void SetShader(const std::string name);
	void SetShader(Shader* shader);

	static int materialCount;
	static std::unordered_map<std::string, Material*> materials;
};

