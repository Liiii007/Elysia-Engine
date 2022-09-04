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
	
	static std::string componentName;
	bool enabled{ false };

	Material(Entity* entity);
	void Init(std::string name, std::string shader);

	//Regist to reflect
	static void Bind();
	//Parse init data from json file
	static void Parse(Entity& entity, const rapidjson::Value& parm);

	virtual void DrawEditorUI() override;

	std::shared_ptr<Shader> getShader();
	void SetShader(const std::string name);

	MaterialConstants& GetMatCB() {
		return data->materialConstants;
	}
	int GetCBIndex() {
		return data->matCBIndex;
	}

	void SetMaterialData(std::string name) {
		if (MaterialData::materialDatas.contains(name)) {
			data = MaterialData::materialDatas[name];
		}
	}

private:
	std::shared_ptr<MaterialData> data;
	std::shared_ptr<Shader> shader;
};