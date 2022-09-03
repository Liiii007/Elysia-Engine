#pragma once
#include <string>
#include <optional>
#include "../../Interface/IComponent.h"
#include "../../Interface/ComponentBase.h"
#include <Resources/MaterialData.h>

class Shader;
class Entity;

class Material : public ComponentBase {

public:
	Material(Entity* entity);

	static std::string componentName;
	bool enabled{ false };
	std::string name;

	Shader* shader;

	void Init(std::string name, std::string shader);

	//Regist to reflect
	static void Bind();
	//Parse init data from json file
	static void Parse(Entity& entity, const rapidjson::Value& parm);

	static Material* GetMaterial(std::string name) {
		if (materials.contains(name)) {
			return materials[name];
		}
		else {
			return nullptr;
		}
	}

	MaterialConstants& GetMatCB() {
		return MaterialData::materialDatas[name]->materialConstants;
	}

	int GetCBIndex() {
		return data->matCBIndex;
	}

	virtual void DrawEditorUI() override;

	Shader* getShader();
	void SetShader(const std::string name);
	void SetShader(Shader* shader);

	static int materialCount;
	static std::unordered_map<std::string, Material*> materials;

private:
	std::shared_ptr<MaterialData> data;
};

